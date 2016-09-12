
#include "ofxAAChannelMetersPanel.h"

//------------------------------------------------
#pragma mark - Core funcs
//------------------------------------------------
void ofxAAChannelMetersPanel::setup(int x, int y, int width, int height, ofxAudioAnalyzerUnit * aaPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    setBackgroundColor(ofColor::teal);
    audioAnalyzer = aaPtr;
    
    _mainColor = ofColor::white;
    
    _bDrawFullDisplay = TRUE;
    
    setupMeters();
    
    ofAddListener(ofxAAMeter::onOffEventGlobal, this, &ofxAAChannelMetersPanel::onMeterStateChanged);

}
//------------------------------------------------
void ofxAAChannelMetersPanel::update(){
    
    //update gui components of all meters-------
    
    for(int i=0; i<meters.size(); i++){
        meters[i]->update();
    }
    
    //set meters values from audioAnalyzer-------
    
    mPower->setValue(audioAnalyzer->getValue(POWER, mPower->getSmoothAmnt()));
    mPitchFreq->setValue(audioAnalyzer->getValue(PITCH_FREQ, mPitchFreq->getSmoothAmnt()));
    mPitchConf->setValue(audioAnalyzer->getValue(PITCH_CONFIDENCE, mPitchConf->getSmoothAmnt()));
    mSalience->setValue(audioAnalyzer->getValue(PITCH_SALIENCE, mSalience->getSmoothAmnt()));
    
    mHfc->setValue(audioAnalyzer->getValue(HFC, mHfc->getSmoothAmnt(), TRUE));
    mCentroid->setValue(audioAnalyzer->getValue(CENTROID, mCentroid->getSmoothAmnt(), TRUE));
    mSpecComp->setValue(audioAnalyzer->getValue(SPECTRAL_COMPLEXITY, mSpecComp->getSmoothAmnt(), TRUE));
    mInharm->setValue(audioAnalyzer->getValue(INHARMONICITY, mInharm->getSmoothAmnt()));
    
    mSpectrum->setValues(audioAnalyzer->getValues(SPECTRUM, mSpectrum->getSmoothAmnt()));
    mMelBands->setValues(audioAnalyzer->getValues(MEL_BANDS, mMelBands->getSmoothAmnt()));
    mMfcc->setValues(audioAnalyzer->getValues(MFCC, mMfcc->getSmoothAmnt()));
    mHpcp->setValues(audioAnalyzer->getValues(HPCP, mHpcp->getSmoothAmnt()));

    mOnsets->setValue(audioAnalyzer->getOnsetValue());
    
    
    
    //set values to std::for osc sending
    
}
//------------------------------------------------
void ofxAAChannelMetersPanel::draw(){
    
    for(int i=0; i<meters.size(); i++){
           meters[i]->draw();
    }

}
//------------------------------------------------
void ofxAAChannelMetersPanel::exit(){
    
    
    delete mPower;
    delete mPitchFreq;
    delete mPitchConf;
    delete mSalience;
    delete mHfc;
    delete mCentroid;
    delete mSpecComp;
    delete mInharm;
    
    delete mOnsets;
    
    delete mSpectrum;
    delete mMelBands;
    delete mMfcc;//dct
    delete mHpcp;
    
    meters.clear();
    
    ofLogVerbose()<<"ofxAAChannelMetersPanel exit.";
}

//------------------------------------------------
#pragma mark - Gral funcs
//------------------------------------------------
void ofxAAChannelMetersPanel::setupMeters(){
    
    VMetersNum = 9; //onsets included
    HMetersNum = 4;
    
    VMetersWidthTotal = _w * VERT_METERS_WIDTH;
    HMetersWidthTotal = _w - VMetersWidthTotal;
    
    VMeterWidth = VMetersWidthTotal / VMetersNum;
    VMeterHeight = _h;
    
    HMeterWidth = HMetersWidthTotal;
    HMeterHeight = _h / HMetersNum;
    
    //-------------------------------------------
    //Vertical Meters
    int x_pos = 0;
    mPower = new ofxAAMeter(MTR_NAME_POWER, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(mPower);
    
    x_pos += VMeterWidth;
    mPitchFreq = new ofxAAMeter(MTR_NAME_PITCH_FREQ, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    mPitchFreq->setMinEstimatedValue(PITCH_MIN_VALUE_FOR_METER);//hz
    mPitchFreq->setMaxEstimatedValue(PITCH_MAX_VALUE_FOR_METER);//hz
    meters.push_back(mPitchFreq);
    
    x_pos += VMeterWidth;
    mPitchConf = new ofxAAMeter(MTR_NAME_PITCH_CONF, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(mPitchConf);
    
    x_pos += VMeterWidth;
    mSalience = new ofxAAMeter(MTR_NAME_PITCH_SALIENCE, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(mSalience);
    
    x_pos += VMeterWidth;
    mHfc = new ofxAAMeter(MTR_NAME_HFC, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(mHfc);
    
    x_pos += VMeterWidth;
    mCentroid = new ofxAAMeter(MTR_NAME_CENTROID, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(mCentroid);
    
    x_pos += VMeterWidth;
    mSpecComp = new ofxAAMeter(MTR_NAME_SPEC_COMP, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(mSpecComp);
    
    x_pos += VMeterWidth;
    mInharm = new ofxAAMeter(MTR_NAME_INHARMONICTY, _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(mInharm);
    
    x_pos += VMeterWidth;
    mOnsets = new ofxAAOnsetMeter(_x + x_pos, _y, VMeterWidth, VMeterHeight, audioAnalyzer);
    meters.push_back(mOnsets);
    
    //Horizontal Meters
    x_pos = VMetersWidthTotal;
    int y_pos = 0;
    mSpectrum = new ofxAABinMeter(MTR_NAME_SPECTRUM, _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mSpectrum->setBinsNum(audioAnalyzer->getBinsNum(SPECTRUM));
    mSpectrum->setMinEstimatedValue(DB_MIN);
    mSpectrum->setMaxEstimatedValue(DB_MAX);
    meters.push_back(mSpectrum);
    
    y_pos += HMeterHeight;
    mMelBands = new ofxAABinMeter(MTR_NAME_MEL_BANDS, _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mMelBands->setBinsNum(audioAnalyzer->getBinsNum(MEL_BANDS));
    mMelBands->setMinEstimatedValue(DB_MIN);
    mMelBands->setMaxEstimatedValue(DB_MAX);
    meters.push_back(mMelBands);
    
    y_pos += HMeterHeight;
    mMfcc = new ofxAABinMeter(MTR_NAME_MFCC, _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mMfcc->setBinsNum(audioAnalyzer->getBinsNum(MFCC));
    mMfcc->setMinEstimatedValue(0.0);
    mMfcc->setMaxEstimatedValue(MFCC_MAX_ESTIMATED_VALUE);
    meters.push_back(mMfcc);
    
    y_pos += HMeterHeight;
    mHpcp = new ofxAABinMeter(MTR_NAME_HPCP, _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mHpcp->setBinsNum(audioAnalyzer->getBinsNum(HPCP));
    meters.push_back(mHpcp);
    
    
    
}

//------------------------------------------------
void ofxAAChannelMetersPanel::setMainColor(ofColor col){
    _mainColor = col;
    for (auto m : meters){
        m->setMainColor(_mainColor);
    }
}
//------------------------------------------------
void ofxAAChannelMetersPanel::setFullDisplay(bool b){
    _bDrawFullDisplay = b;
    for (auto m : meters){
        m->setFullDisplay(_bDrawFullDisplay);
    }
}
//------------------------------------------------
void ofxAAChannelMetersPanel::adjustPosAndHeight(int y, int h){
    
    _y = y;
    _h = h;
    
    HMeterHeight = _h / HMetersNum;
    
    int y_pos = 0;
    
    for(int i=0; i<meters.size(); i++){
        
        if(meters[i]->getMeterOrient()==VERTICAL){
            meters[i]->setYandHeight(_y, _h);
        }else{
            meters[i]->setYandHeight(_y + y_pos, HMeterHeight);
            y_pos += HMeterHeight;
        }
    }
    
    
}


//--------------------------------------------------------------
void ofxAAChannelMetersPanel::onMeterStateChanged(OnOffEventData & data){
    
    //cout << "panel: "<<data.name << endl;
    //cout << "panel: "<<data.state << endl;

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
        audioAnalyzer->setActive(MEL_BANDS, data.state);
    }else if (data.name == MTR_NAME_MFCC){
        audioAnalyzer->setActive(MFCC, data.state);
    }else if(data.name == MTR_NAME_HPCP){
        audioAnalyzer->setActive(HPCP, data.state);
    }
    
}
//--------------------------------------------------------------
#pragma mark - Settings funcs
//--------------------------------------------------------------
void ofxAAChannelMetersPanel::loadSettingsFromFile(string filename){
    
    ofxXmlSettings xml;
    
    if( xml.loadFile(filename) ){
        ofLogVerbose()<<"ofxAAChannelMetersPanel: "<< filename <<" loaded.";
    }else{
        ofLogError()<< "ofxAAChannelMetersPanel: unable to load " << filename ;
        return;
    }
    //int numDragTags = XML.getNumTags("PANEL");
    //cout<<""<<xml.getValue("PANEL:POWER:SMOOTH", 0.0)<<endl;
    
    
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
    
    mOnsets->setAlpha(xml.getValue("PANEL:ONSETS:ALPHA", 0.0));
    mOnsets->setSilenceTreshold(xml.getValue("PANEL:ONSETS:SILENCETRESHOLD", 0.0));
    mOnsets->setTimeTreshold(xml.getValue("PANEL:ONSETS:TIMETRESHOLD", 0.0));
    state = xml.getValue("PANEL:ONSETS:STATE", 0) > 0;
    mOnsets->setEnabled(state);
    audioAnalyzer->setActive(ONSETS, state);
    
    mSpectrum->setSmoothAmnt(xml.getValue("PANEL:SPECTRUM:SMOOTH", 0.0));
    state = xml.getValue("PANEL:SPECTRUM:STATE", 0) > 0;
    mSpectrum->setEnabled(state);
    //spectrm cant be turned off
    
    mMelBands->setSmoothAmnt(xml.getValue("PANEL:MELBANDS:SMOOTH", 0.0));
    state = xml.getValue("PANEL:MELBANDS:STATE", 0) > 0;
    mMelBands->setEnabled(state);
    audioAnalyzer->setActive(MEL_BANDS, state);//linked to mfcc
    
    mMfcc->setSmoothAmnt(xml.getValue("PANEL:MFCC:SMOOTH", 0.0));
    state = xml.getValue("PANEL:MFCC:STATE", 0) > 0;
    mMfcc->setEnabled(state);
    audioAnalyzer->setActive(MFCC, state);//linked to melbands
    
    mHpcp->setSmoothAmnt(xml.getValue("PANEL:HPCP:SMOOTH", 0.0));
    state = xml.getValue("PANEL:HPCP:STATE", 0) > 0;
    mHpcp->setEnabled(state);
    audioAnalyzer->setActive(HPCP, state);
    
}
//--------------------------------------------------------------
void ofxAAChannelMetersPanel::saveSettingsToFile(string filename){
    
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
    
    savedSettings.addTag("ONSETS");
    savedSettings.pushTag("ONSETS");
    savedSettings.addValue("ALPHA", mOnsets->getAlpha());
    savedSettings.addValue("SILENCETRESHOLD",mOnsets->getSilenceTreshold());
    savedSettings.addValue("TIMETRESHOLD", mOnsets->getTimeTreshold());
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
    
    savedSettings.saveFile(filename);
    
}
