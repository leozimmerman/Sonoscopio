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

#pragma mark - Core funcs

ChannelMetersView::ChannelMetersView(int x, int y, int width, int height, int panelId, ofxAudioAnalyzerUnit * audioAnalyzer, vector<ofxAAAlgorithmType>& enabledAlgorithms, ofColor mainColor){
   
    View::setup(x, y ,width, height);
    
    ofAddListener(MeterView::onOffEventGlobal, this, &ChannelMetersView::onMeterStateChanged);
   
    _panelId = panelId;
    audioAnalyzerUnit = audioAnalyzer;
    _enabledAlgorithmTypes = enabledAlgorithms;
    _mainColor = mainColor;
    
    createMeters();
}

void ChannelMetersView::createMeters(){
    meters.clear();
    for (auto type : _enabledAlgorithmTypes) {
        auto meterView = MeterView::createMeterView(type, _panelId, audioAnalyzerUnit);
        meters.push_back(meterView);
    }
    setColors();
    resize(_x, _y, _w, _h);
    
}

void ChannelMetersView::setEnabledAlgorithms(vector<ofxAAAlgorithmType> &enabledAlgorithms){
    _enabledAlgorithmTypes = enabledAlgorithms;
    createMeters();
}

void ChannelMetersView::update(){
    for(int i=0; i<meters.size(); i++){
       meters[i]->update();
    }
}

void ChannelMetersView::draw(){
    View::draw();
    for(int i=0; i<meters.size(); i++){
        meters[i]->draw();
    }
}

void ChannelMetersView::exit(){
    for (auto m : meters) {
        delete m;
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
    resize(_x, _y, _w, _h);
}
//------------------------------------------------
void ChannelMetersView::setColors(){
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
    int metersWidth = _w;
    
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
    
    audioAnalyzerUnit->setActive(data.type, data.state);
    
    /*
     TODO: Revisar cuando compile...
    if(data.name == MTR_NAME_MEL_BANDS){
        mMfcc->setEnabled(data.state);//if MelBands are off, Mfcc must be off too.
        //audioAnalyzer->setActive(MEL_BANDS, data.state);
        audioAnalyzer->setActive(MFCC, data.state);//turns on-off mfcc and mel-bands together
    }
     */
    
}

#pragma mark - Settings

void ChannelMetersView::loadSettings(ChannelMeterSettings& settings){
    currentSettings = settings;
    for (auto ms : currentSettings.meters) {
        string stringType = ms.type;
        ofxAAAlgorithmType type = ofxaa::stringToAlgorithmType(stringType);
        
        auto meter = meterForType(type);
        
        meter->setEnabled(ms.bState);
        if (type == ONSETS) {
            auto onsets_m = dynamic_cast<OnsetMeterView*>(meter);
            onsets_m->setAlpha(ms.alpha);
            onsets_m->setSilenceThreshold(ms.silenceTreshold);
            onsets_m->setTimeThreshold(ms.timeTreshold);
        } else {
            meter->setSmoothAmnt(ms.smooth);
        }
    
        //spectrm cant be turned off, no audioAnalyzer->setActive
        //MFcc also turn on-off melBands
        audioAnalyzerUnit->setActive(type, ms.bState);
    }
}

void ChannelMetersView::updateCurrentSettings(){
    currentSettings.meters.clear();
    for (auto m : meters) {
        MeterSettings setting;
        
        auto type = m->getType();
        auto typeString = ofxaa::algorithmTypeToString(type);
        
        setting.type = typeString;
        
        if (type == ONSETS) {
            auto onsets_m = dynamic_cast<OnsetMeterView*>(m);
            setting.alpha = onsets_m->getAlpha();
            setting.silenceTreshold = onsets_m->getSilenceThreshold();
            setting.timeTreshold = onsets_m->getTimeThreshold();
            setting.bState = onsets_m->getEnabled();
        } else {
            setting.smooth = m->getSmoothAmnt();
            setting.bState = m->getEnabled();
        }
        currentSettings.meters.push_back(setting);
    }
}

//for osc and data saving
MeterView* ChannelMetersView::meterForType(ofxAAAlgorithmType type) {
    for (auto m: meters) {
        if (m->getType() == type) {
            return m;
        }
    }
    return NULL;
}
