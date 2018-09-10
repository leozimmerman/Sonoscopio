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

//------------------------------------------------
#pragma mark - Core funcs
//------------------------------------------------
ChannelMetersView::ChannelMetersView(int x, int y, int width, int height, int panelId, ofxAudioAnalyzerUnit * aaPtr){

    View::setup(x, y ,width, height);
    
    setBackgroundColor(ofColor::orange);
   
    _panelId = panelId;

    audioAnalyzer = aaPtr;

    _mainColor = ofColor::white;

    _bDrawFullDisplay = TRUE;

    setupMeters();

    ofAddListener(MeterView::onOffEventGlobal, this, &ChannelMetersView::onMeterStateChanged);
    
}

//------------------------------------------------
void ChannelMetersView::update(){
    
    //-:Update gui components of all meters
    for(int i=0; i<meters.size(); i++){
        meters[i]->update();
    }
    
    //FIXME: hacerlo gral.
    //-:Set meters values from audioAnalyzer:
    mPower->setValue(audioAnalyzer->getValue(POWER, mPower->getSmoothAmnt()));
    
    mPitchFreq->setValue(audioAnalyzer->getValue(PITCH_FREQ, mPitchFreq->getSmoothAmnt()));
    mPitchFreq->setNormalizedValue(audioAnalyzer->getValue(PITCH_FREQ, mPitchFreq->getSmoothAmnt(), TRUE));
    
    mPitchConf->setValue(audioAnalyzer->getValue(PITCH_CONFIDENCE, mPitchConf->getSmoothAmnt()));
    mSalience->setValue(audioAnalyzer->getValue(PITCH_SALIENCE, mSalience->getSmoothAmnt()));
    
    mHfc->setValue(audioAnalyzer->getValue(HFC, mHfc->getSmoothAmnt()));
    mHfc->setNormalizedValue(audioAnalyzer->getValue(HFC, mHfc->getSmoothAmnt(), TRUE));
    
    mCentroid->setValue(audioAnalyzer->getValue(CENTROID, mCentroid->getSmoothAmnt()));
    mCentroid->setNormalizedValue(audioAnalyzer->getValue(CENTROID, mCentroid->getSmoothAmnt(), TRUE));
    
    mSpecComp->setValue(audioAnalyzer->getValue(SPECTRAL_COMPLEXITY, mSpecComp->getSmoothAmnt()));
    mSpecComp->setNormalizedValue(audioAnalyzer->getValue(SPECTRAL_COMPLEXITY, mSpecComp->getSmoothAmnt(), TRUE));
    
    mInharm->setValue(audioAnalyzer->getValue(INHARMONICITY, mInharm->getSmoothAmnt()));
    
    mDissonance->setValue(audioAnalyzer->getValue(DISSONANCE, mDissonance->getSmoothAmnt()));
    
    mRollOff->setValue(audioAnalyzer->getValue(ROLL_OFF, mRollOff->getSmoothAmnt()));
    mRollOff->setNormalizedValue(audioAnalyzer->getValue(ROLL_OFF, mRollOff->getSmoothAmnt(), TRUE));
    
    mOddToEven->setValue(audioAnalyzer->getValue(ODD_TO_EVEN, mOddToEven->getSmoothAmnt()));
    mOddToEven->setNormalizedValue(audioAnalyzer->getValue(ODD_TO_EVEN, mOddToEven->getSmoothAmnt(), TRUE));
    
    mSpectrum->setValues(audioAnalyzer->getValues(SPECTRUM, mSpectrum->getSmoothAmnt()));
    mMelBands->setValues(audioAnalyzer->getValues(MEL_BANDS, mMelBands->getSmoothAmnt()));
    mMfcc->setValues(audioAnalyzer->getValues(MFCC, mMfcc->getSmoothAmnt()));
    mHpcp->setValues(audioAnalyzer->getValues(HPCP, mHpcp->getSmoothAmnt()));
    
    mTristimulus->setValues(audioAnalyzer->getValues(TRISTIMULUS, mTristimulus->getSmoothAmnt()));

    mOnsets->setValue(audioAnalyzer->getOnsetValue());
    
    //set values to std::for osc sending
    
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
    
    //FIXME: hacerlo gral.
    delete mPower;
    delete mPitchFreq;
    delete mPitchConf;
    delete mSalience;
    delete mHfc;
    delete mCentroid;
    delete mSpecComp;
    delete mInharm;
    
    delete mDissonance;
    delete mRollOff;
    delete mOddToEven;
    
    delete mOnsets;
    
    delete mSpectrum;
    delete mMelBands;
    delete mMfcc;//dct
    delete mHpcp;
    delete mTristimulus;
    
    meters.clear();
    
    ofRemoveListener(MeterView::onOffEventGlobal, this, &ChannelMetersView::onMeterStateChanged);
    
    ofLogVerbose()<<"ofxAAChannelMetersPanel exit.";
}

//------------------------------------------------
#pragma mark - Gral funcs
//------------------------------------------------
void ChannelMetersView::setupMeters(){
    metersNum = 17;
    metersWidth = _w;
    metersHeight = _h / metersNum;
    //FIXME: hacerlo gral.
    //Vertical Meters
    int y_pos = 0;
    mPower = new MeterView(MTR_NAME_POWER, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mPower);
    
    y_pos += metersHeight;
    mPitchFreq = new MeterView(MTR_NAME_PITCH_FREQ,_x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mPitchFreq);
    
    y_pos += metersHeight;
    mPitchConf = new MeterView(MTR_NAME_PITCH_CONF, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mPitchConf);
    
    y_pos += metersHeight;
    mSalience = new MeterView(MTR_NAME_PITCH_SALIENCE, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mSalience);
    
    y_pos += metersHeight;
    mHfc = new MeterView(MTR_NAME_HFC, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mHfc);
    
    y_pos += metersHeight;
    mCentroid = new MeterView(MTR_NAME_CENTROID, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mCentroid);
    
   y_pos += metersHeight;
    mSpecComp = new MeterView(MTR_NAME_SPEC_COMP, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mSpecComp);
    
    y_pos += metersHeight;
    mInharm = new MeterView(MTR_NAME_INHARMONICTY, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mInharm);
    
    y_pos += metersHeight;
    mDissonance = new MeterView(MTR_NAME_DISSONANCE,_x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mDissonance);
    
    y_pos += metersHeight;
    mRollOff = new MeterView(MTR_NAME_ROLL_OFF, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mRollOff);
    
    y_pos += metersHeight;
    mOddToEven = new MeterView(MTR_NAME_ODD_TO_EVEN, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    meters.push_back(mOddToEven);
    
    y_pos += metersHeight;
    mOnsets = new OnsetMeterView(_x, _y + y_pos, metersWidth, metersHeight, _panelId, audioAnalyzer);
    meters.push_back(mOnsets);
    
    //Horizontal Meters
    
    y_pos += metersHeight;
    mSpectrum = new BinMeterView(MTR_NAME_SPECTRUM,_x, _y + y_pos, metersWidth, metersHeight, _panelId);
    mSpectrum->setBinsNum(audioAnalyzer->getBinsNum(SPECTRUM));
    mSpectrum->setMinEstimatedValue(DB_MIN);
    mSpectrum->setMaxEstimatedValue(DB_MAX);
    meters.push_back(mSpectrum);
    
    y_pos += metersHeight;
    mMelBands = new BinMeterView(MTR_NAME_MEL_BANDS, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    mMelBands->setBinsNum(audioAnalyzer->getBinsNum(MEL_BANDS));
    mMelBands->setMinEstimatedValue(DB_MIN);
    mMelBands->setMaxEstimatedValue(DB_MAX);
    meters.push_back(mMelBands);
    
    y_pos += metersHeight;
    mMfcc = new BinMeterView(MTR_NAME_MFCC, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    mMfcc->setBinsNum(audioAnalyzer->getBinsNum(MFCC));
    mMfcc->setMinEstimatedValue(0.0);
    mMfcc->setMaxEstimatedValue(MFCC_MAX_ESTIMATED_VALUE);
    meters.push_back(mMfcc);
    
    y_pos += metersHeight;
    mHpcp = new BinMeterView(MTR_NAME_HPCP, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    mHpcp->setBinsNum(audioAnalyzer->getBinsNum(HPCP));
    meters.push_back(mHpcp);
    
    y_pos += metersHeight;
    mTristimulus = new BinMeterView(MTR_NAME_TRISTIMULUS, _x, _y + y_pos, metersWidth, metersHeight, _panelId);
    mTristimulus->setBinsNum(TRISTIMULUS_BANDS_NUM);
    meters.push_back(mTristimulus);
    
    
}

//------------------------------------------------
void ChannelMetersView::setMainColor(ofColor col){
    _mainColor = col;
    
    ofColor lightCol = _mainColor;
    lightCol.setBrightness(30);//darker mainColor
    setBackgroundColor(lightCol);
    
    for (auto m : meters){
        
        m->setMainColor(_mainColor);
        
        //update onsets sliders colors
        if(m->getName()==MTR_NAME_ONSETS){
            OnsetMeterView* om = dynamic_cast<OnsetMeterView*>(m);
            om->updateComponentsColors();
        }
    }
}
//------------------------------------------------
void ChannelMetersView::setFullDisplay(bool b){
    _bDrawFullDisplay = b;
    for (auto m : meters){
        m->setFullDisplay(_bDrawFullDisplay);
    }
}
//------------------------------------------------
void ChannelMetersView::resize(int x, int y, int w, int h) {
    View::resize(x, y, w, h);
    _w = w ;
    metersNum = 17;
    metersWidth = _w;
    metersHeight = _h / metersNum;
    
    int y_pos = 0;
    for (auto m : meters) {
        m->resize(_x, _y + y_pos, metersWidth, metersHeight);
        y_pos += metersHeight;
    }
}

//--------------------------------------------------------------
///listener for all on/off buttons of all pannels
void ChannelMetersView::onMeterStateChanged(OnOffEventData & data){
    
    if(data.panelId != _panelId) return;
    
    //-----------------------
    //FIXME: hacerlo gral.
    if(data.name == MTR_NAME_POWER){
        audioAnalyzer->setActive(POWER, data.state);
    }else if(data.name == MTR_NAME_PITCH_FREQ){
        audioAnalyzer->setActive(PITCH_FREQ, data.state);
    }else if(data.name == MTR_NAME_PITCH_CONF){
        audioAnalyzer->setActive(PITCH_CONFIDENCE, data.state);
    }else if(data.name == MTR_NAME_PITCH_SALIENCE){
        audioAnalyzer->setActive(PITCH_SALIENCE, data.state);
    }else if(data.name == MTR_NAME_HFC){
        audioAnalyzer->setActive(HFC, data.state);
    }else if(data.name == MTR_NAME_CENTROID){
        audioAnalyzer->setActive(CENTROID, data.state);
    }else if(data.name == MTR_NAME_SPEC_COMP){
        audioAnalyzer->setActive(SPECTRAL_COMPLEXITY, data.state);
    }else if(data.name == MTR_NAME_INHARMONICTY){
        audioAnalyzer->setActive(INHARMONICITY, data.state);
    }else if(data.name == MTR_NAME_ONSETS){
        audioAnalyzer->setActive(ONSETS, data.state);
    }else if(data.name == MTR_NAME_MEL_BANDS){
        mMfcc->setEnabled(data.state);//if MelBands are off, Mfcc must be off too.
        //audioAnalyzer->setActive(MEL_BANDS, data.state);
        audioAnalyzer->setActive(MFCC, data.state);//turns on-off mfcc and mel-bands together
    }else if (data.name == MTR_NAME_MFCC){
        //-This toggle is hidden
        audioAnalyzer->setActive(MFCC, data.state);
    }else if(data.name == MTR_NAME_HPCP){
        audioAnalyzer->setActive(HPCP, data.state);
    }else if(data.name == MTR_NAME_DISSONANCE){
        audioAnalyzer->setActive(DISSONANCE, data.state);
    }else if(data.name == MTR_NAME_ROLL_OFF){
        audioAnalyzer->setActive(ROLL_OFF, data.state);
    }else if(data.name == MTR_NAME_ODD_TO_EVEN){
        audioAnalyzer->setActive(ODD_TO_EVEN, data.state);
    }else if(data.name == MTR_NAME_TRISTIMULUS){
        audioAnalyzer->setActive(TRISTIMULUS, data.state);
    }
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
    
    //FIXME: hacerlo gral.
    mPower->setSmoothAmnt(xml.getValue("PANEL:POWER:SMOOTH", 0.0));
    bool state = xml.getValue("PANEL:POWER:STATE", 0) > 0;
    mPower->setEnabled(state);
    audioAnalyzer->setActive(POWER, state);
    
    mPitchFreq->setSmoothAmnt(xml.getValue("PANEL:PITCHFREQ:SMOOTH", 0.0));
    state = xml.getValue("PANEL:PITCHFREQ:STATE", 0) > 0;
    mPitchFreq->setEnabled(state);
    audioAnalyzer->setActive(PITCH_FREQ, state);
    
    mPitchConf->setSmoothAmnt(xml.getValue("PANEL:PITCHCONF:SMOOTH", 0.0));
    state = xml.getValue("PANEL:PITCHCONF:STATE", 0) > 0;
    mPitchConf->setEnabled(state);
    audioAnalyzer->setActive(PITCH_CONFIDENCE, state);
    
    mSalience->setSmoothAmnt(xml.getValue("PANEL:SALIENCE:SMOOTH", 0.0));
    state = xml.getValue("PANEL:SALIENCE:STATE", 0) > 0;
    mSalience->setEnabled(state);
    audioAnalyzer->setActive(PITCH_SALIENCE, state);
    
    mHfc->setSmoothAmnt(xml.getValue("PANEL:HFC:SMOOTH", 0.0));
    state = xml.getValue("PANEL:HFC:STATE", 0) > 0;
    mHfc->setEnabled(state);
    audioAnalyzer->setActive(HFC, state);
    
    mCentroid->setSmoothAmnt(xml.getValue("PANEL:CENTROID:SMOOTH", 0.0));
    state = xml.getValue("PANEL:CENTROID:STATE", 0) > 0;
    mCentroid->setEnabled(state);
    audioAnalyzer->setActive(CENTROID, state);
    
    mSpecComp->setSmoothAmnt(xml.getValue("PANEL:SPECCOMP:SMOOTH", 0.0));
    state = xml.getValue("PANEL:SPECCOMP:STATE", 0) > 0;
    mSpecComp->setEnabled(state);
    audioAnalyzer->setActive(SPECTRAL_COMPLEXITY, state);
    
    mInharm->setSmoothAmnt(xml.getValue("PANEL:INHARM:SMOOTH", 0.0));
    state = xml.getValue("PANEL:INHARM:STATE", 0) > 0;
    mInharm->setEnabled(state);
    audioAnalyzer->setActive(INHARMONICITY, state);
    
    mDissonance->setSmoothAmnt(xml.getValue("PANEL:DISSONANCE:SMOOTH", 0.0));
    state = xml.getValue("PANEL:DISSONANCE:STATE", 0) > 0;
    mDissonance->setEnabled(state);
    audioAnalyzer->setActive(DISSONANCE, state);
    
    mRollOff->setSmoothAmnt(xml.getValue("PANEL:ROLLOFF:SMOOTH", 0.0));
    state = xml.getValue("PANEL:ROLLOFF:STATE", 0) > 0;
    mRollOff->setEnabled(state);
    audioAnalyzer->setActive(ROLL_OFF, state);
    
    mOddToEven->setSmoothAmnt(xml.getValue("PANEL:ODDTOEVEN:SMOOTH", 0.0));
    state = xml.getValue("PANEL:ODDTOEVEN:STATE", 0) > 0;
    mOddToEven->setEnabled(state);
    audioAnalyzer->setActive(ODD_TO_EVEN, state);
    
    mOnsets->setAlpha(xml.getValue("PANEL:ONSETS:ALPHA", 0.0));
    mOnsets->setSilenceThreshold(xml.getValue("PANEL:ONSETS:SILENCEThreshold", 0.0));
    mOnsets->setTimeThreshold(xml.getValue("PANEL:ONSETS:TIMEThreshold", 0.0));
    state = xml.getValue("PANEL:ONSETS:STATE", 0) > 0;
    mOnsets->setEnabled(state);
    audioAnalyzer->setActive(ONSETS, state);
    
    mSpectrum->setSmoothAmnt(xml.getValue("PANEL:SPECTRUM:SMOOTH", 0.0));
    state = xml.getValue("PANEL:SPECTRUM:STATE", 0) > 0;
    mSpectrum->setEnabled(state);
    //spectrm cant be turned off, no audioAnalyzer->setActive
    
    //-MelBands and Mfcc must have the same state
    mMelBands->setSmoothAmnt(xml.getValue("PANEL:MELBANDS:SMOOTH", 0.0));
    state = xml.getValue("PANEL:MELBANDS:STATE", 0) > 0;
    mMelBands->setEnabled(state);
    ///audioAnalyzer->setActive(MEL_BANDS, state);
    
    mMfcc->setSmoothAmnt(xml.getValue("PANEL:MFCC:SMOOTH", 0.0));
    state = xml.getValue("PANEL:MFCC:STATE", 0) > 0;
    mMfcc->setEnabled(state);
    audioAnalyzer->setActive(MFCC, state);//also turn on-off melBands
    //--
    
    mHpcp->setSmoothAmnt(xml.getValue("PANEL:HPCP:SMOOTH", 0.0));
    state = xml.getValue("PANEL:HPCP:STATE", 0) > 0;
    mHpcp->setEnabled(state);
    audioAnalyzer->setActive(HPCP, state);
    
    mTristimulus->setSmoothAmnt(xml.getValue("PANEL:TRISTIMULUS:SMOOTH", 0.0));
    state = xml.getValue("PANEL:TRISTIMULUS:STATE", 0) > 0;
    mTristimulus->setEnabled(state);
    audioAnalyzer->setActive(TRISTIMULUS, state);
    
}
//--------------------------------------------------------------
void ChannelMetersView::saveSettingsToFile(string filename){
    //FIXME: hacerlo gral. ???
    ofxXmlSettings savedSettings;
    savedSettings.addTag("PANEL");
    savedSettings.pushTag("PANEL");
    
    savedSettings.addTag("POWER");
    savedSettings.pushTag("POWER");
    savedSettings.addValue("SMOOTH", mPower->getSmoothAmnt());
    savedSettings.addValue("STATE", mPower->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("PITCHFREQ");
    savedSettings.pushTag("PITCHFREQ");
    savedSettings.addValue("SMOOTH",  mPitchFreq->getSmoothAmnt());
    savedSettings.addValue("STATE", mPitchFreq->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("PITCHCONF");
    savedSettings.pushTag("PITCHCONF");
    savedSettings.addValue("SMOOTH", mPitchConf->getSmoothAmnt());
    savedSettings.addValue("STATE", mPitchConf->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("SALIENCE");
    savedSettings.pushTag("SALIENCE");
    savedSettings.addValue("SMOOTH", mSalience->getSmoothAmnt());
    savedSettings.addValue("STATE", mSalience->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("HFC");
    savedSettings.pushTag("HFC");
    savedSettings.addValue("SMOOTH", mHfc->getSmoothAmnt());
    savedSettings.addValue("STATE", mHfc->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("CENTROID");
    savedSettings.pushTag("CENTROID");
    savedSettings.addValue("SMOOTH", mCentroid->getSmoothAmnt());
    savedSettings.addValue("STATE", mCentroid->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("SPECCOMP");
    savedSettings.pushTag("SPECCOMP");
    savedSettings.addValue("SMOOTH", mSpecComp->getSmoothAmnt());
    savedSettings.addValue("STATE", mSpecComp->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("INHARM");
    savedSettings.pushTag("INHARM");
    savedSettings.addValue("SMOOTH", mInharm->getSmoothAmnt());
    savedSettings.addValue("STATE", mInharm->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("DISSONANCE");
    savedSettings.pushTag("DISSONANCE");
    savedSettings.addValue("SMOOTH", mDissonance->getSmoothAmnt());
    savedSettings.addValue("STATE", mDissonance->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("ROLLOFF");
    savedSettings.pushTag("ROLLOFF");
    savedSettings.addValue("SMOOTH", mRollOff->getSmoothAmnt());
    savedSettings.addValue("STATE", mRollOff->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("ODDTOEVEN");
    savedSettings.pushTag("ODDTOEVEN");
    savedSettings.addValue("SMOOTH", mOddToEven->getSmoothAmnt());
    savedSettings.addValue("STATE", mOddToEven->getEnabled());
    savedSettings.popTag();
    
    
    savedSettings.addTag("ONSETS");
    savedSettings.pushTag("ONSETS");
    savedSettings.addValue("ALPHA", mOnsets->getAlpha());
    savedSettings.addValue("SILENCEThreshold",mOnsets->getSilenceThreshold());
    savedSettings.addValue("TIMEThreshold", mOnsets->getTimeThreshold());
    savedSettings.addValue("STATE", mOnsets->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("SPECTRUM");
    savedSettings.pushTag("SPECTRUM");
    savedSettings.addValue("SMOOTH", mSpectrum->getSmoothAmnt());
    savedSettings.addValue("STATE", mSpectrum->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("MELBANDS");
    savedSettings.pushTag("MELBANDS");
    savedSettings.addValue("SMOOTH", mMelBands->getSmoothAmnt());
    savedSettings.addValue("STATE", mMelBands->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("MFCC");
    savedSettings.pushTag("MFCC");
    savedSettings.addValue("SMOOTH", mMfcc->getSmoothAmnt());
    savedSettings.addValue("STATE", mMfcc->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("HPCP");
    savedSettings.pushTag("HPCP");
    savedSettings.addValue("SMOOTH", mHpcp->getSmoothAmnt());
    savedSettings.addValue("STATE", mHpcp->getEnabled());
    savedSettings.popTag();
    
    savedSettings.addTag("TRISTIMULUS");
    savedSettings.pushTag("TRISTIMULUS");
    savedSettings.addValue("SMOOTH", mTristimulus->getSmoothAmnt());
    savedSettings.addValue("STATE", mTristimulus->getEnabled());
    savedSettings.popTag();
    
    savedSettings.saveFile(filename);
    
}
