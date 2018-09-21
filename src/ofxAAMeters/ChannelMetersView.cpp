/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * Sonoscopio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#include "ChannelMetersView.h"
#include "ofxAAUtils.h"

//TODO: Make it static-global

vector<ofxAAAlgorithmType> availableTypes = {RMS, PITCH_FREQ, PITCH_CONFIDENCE, PITCH_SALIENCE, HFC, CENTROID, SPECTRAL_COMPLEXITY, INHARMONICITY, DISSONANCE, ROLL_OFF, ODD_TO_EVEN, ONSETS, SPECTRUM, MEL_BANDS, MFCC, HPCP, TRISTIMULUS};


//------------------------------------------------
#pragma mark - Core funcs
//------------------------------------------------
ChannelMetersView::ChannelMetersView(int x, int y, int width, int height, int panelId, ofxAudioAnalyzerUnit * aaPtr){
    View::setup(x, y ,width, height);
    
    setBackgroundColor(ofColor::yellow);
   
    _panelId = panelId;
    audioAnalyzer = aaPtr;
    _mainColor = ofColor::yellow;

    initMeters();

    ofAddListener(MeterView::onOffEventGlobal, this, &ChannelMetersView::onMeterStateChanged);
    resize(_x, _y, _w, _h);
    
}
//------------------------------------------------
void ChannelMetersView::initMeters(){
    for (auto type : availableTypes) {
        auto meterView = MeterView::createMeterView(type, _panelId, audioAnalyzer);
        meters.push_back(meterView);
    }
    resize(_x, _y, _w, _h);
}
//------------------------------------------------
void ChannelMetersView::update(){
    for(int i=0; i<meters.size(); i++){
       meters[i]->update();
    }
}
//------------------------------------------------
void ChannelMetersView::draw(){
    View::draw();
    for(int i=0; i<meters.size(); i++){
        meters[i]->draw();
    }
}
//------------------------------------------------
void ChannelMetersView::exit(){
    
    //TODO: Make shared_ptr
    for (auto m : meters) {
        delete m;//FIXME: ! Esto se puede??
    }

    meters.clear();
    
    ofRemoveListener(MeterView::onOffEventGlobal, this, &ChannelMetersView::onMeterStateChanged);
    
    ofLogVerbose()<<"ofxAAChannelMetersPanel exit.";
}

//------------------------------------------------
#pragma mark - Gral funcs
//------------------------------------------------
void ChannelMetersView::scrollUp(){
    _scrollOffset += 40;
    if (_scrollOffset > 0) {_scrollOffset = 0;}
    resize(_x , _y, _w, _h);
}
//------------------------------------------------
void ChannelMetersView::scrollDown(){
    _scrollOffset -= 40;
    
    if (_scrollOffset < (_contentHeight-_h) * (-1)) {
        _scrollOffset = (_contentHeight-_h) * (-1);
    }
    resize(_x , _y, _w, _h);
}
//------------------------------------------------
void ChannelMetersView::setMainColor(ofColor col){
    _mainColor = col;
    ofColor lightCol = _mainColor;
    lightCol.setBrightness(30);//darker mainColor
    setBackgroundColor(lightCol);
    for (auto m : meters){
        m->setMainColor(_mainColor);
        if(m->getType()==ONSETS){
            OnsetMeterView* om = dynamic_cast<OnsetMeterView*>(m);
            om->updateComponentsColors();//update sliders colors
        }
    }
}
//------------------------------------------------
int ChannelMetersView::getHeightForMeter(MeterView *meter) {
    if (dynamic_cast<BinMeterView*>(meter)) {
        return BinMeterView::height;
    } else if (dynamic_cast<OnsetMeterView*>(meter))  {
        return OnsetMeterView::height;
    } else {
        return MeterView::height;
    }
}
//------------------------------------------------
void ChannelMetersView::resize(int x, int y, int w, int h) {
    View::resize(x, y, w, h);
    metersNum = availableTypes.size();
    metersWidth = _w;
    
    int y_pos = 0;
    for (auto m : meters) {
        int h = getHeightForMeter(m);
        m->resize(_x, _y + y_pos + _scrollOffset, metersWidth, h);
        y_pos += h;
    }
    _contentHeight = y_pos;
}

//--------------------------------------------------------------
///listener for all on/off buttons of all pannels
void ChannelMetersView::onMeterStateChanged(OnOffEventData & data){
    
    if(data.panelId != _panelId) return;
    
    audioAnalyzer->setActive(data.type, data.state);
    
    /*
     TODO: Revisar cuando compile...
    if(data.name == MTR_NAME_MEL_BANDS){
        mMfcc->setEnabled(data.state);//if MelBands are off, Mfcc must be off too.
        //audioAnalyzer->setActive(MEL_BANDS, data.state);
        audioAnalyzer->setActive(MFCC, data.state);//turns on-off mfcc and mel-bands together
    }
     */
    
}
//--------------------------------------------------------------
#pragma mark - Settings funcs
//--------------------------------------------------------------
void ChannelMetersView::loadSettingsFromFile(string filename){
    
    ofxXmlSettings xml;
    
    if( xml.loadFile(filename) ){
        ofLogVerbose()<<"ofxAAChannelMetersPanel: "<< filename <<" loaded.";
    }else{
        ofLogError()<< "ofxAAChannelMetersPanel: unable to load " << filename ;
        return;
    }
    //int numDragTags = XML.getNumTags("PANEL");
    //cout<<""<<xml.getValue("PANEL:POWER:SMOOTH", 0.0)<<endl;
    
    for (auto m : meters) {
        auto type = m->getType();
        auto str = ofxaa::algorithmTypeToString(type);
    
        m->setSmoothAmnt(xml.getValue("PANEL:" + str + ":SMOOTH", 0.0));
        bool state = xml.getValue("PANEL:" + str + ":STATE", 0) > 0;
        m->setEnabled(state);
        //spectrm cant be turned off, no audioAnalyzer->setActive
        //MFcc also turn on-off melBands
        audioAnalyzer->setActive(type, state);
        if (type == ONSETS) {
            auto onsets_m = dynamic_cast<OnsetMeterView*>(m);
            onsets_m->setAlpha(xml.getValue("PANEL:" + str + ":ALPHA", 0.0));
            onsets_m->setSilenceThreshold(xml.getValue("PANEL:" + str + ":SILENCEThreshold", 0.0));
            onsets_m->setTimeThreshold(xml.getValue("PANEL:" + str + ":TIMEThreshold", 0.0));
        }
    }
}
//--------------------------------------------------------------
void ChannelMetersView::saveSettingsToFile(string filename){
    
    ofxXmlSettings savedSettings;
    savedSettings.addTag("PANEL");
    savedSettings.pushTag("PANEL");
    
    for (auto m : meters) {
        auto type = m->getType();
        auto str = ofxaa::algorithmTypeToString(type);
        if (type == ONSETS) {
            auto onsets_m = dynamic_cast<OnsetMeterView*>(m);
            savedSettings.addTag(str);
            savedSettings.pushTag(str);
            savedSettings.addValue("ALPHA", onsets_m->getAlpha());
            savedSettings.addValue("SILENCEThreshold",onsets_m->getSilenceThreshold());
            savedSettings.addValue("TIMEThreshold", onsets_m->getTimeThreshold());
            savedSettings.addValue("STATE", onsets_m->getEnabled());
            savedSettings.popTag();
        } else {
            savedSettings.addTag(str);
            savedSettings.pushTag(str);
            savedSettings.addValue("SMOOTH", m->getSmoothAmnt());
            savedSettings.addValue("STATE", m->getEnabled());
            savedSettings.popTag();
        }
    }
    savedSettings.saveFile(filename);
}
