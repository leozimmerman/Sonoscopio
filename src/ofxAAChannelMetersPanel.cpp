
#include "ofxAAChannelMetersPanel.h"

void ofxAAChannelMetersPanel::setup(int x, int y, int width, int height, ofxAudioAnalyzer * aaPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    setBackgroundColor(ofColor::darkorange);
    audioAnalyzer = aaPtr;
    
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
    
    mPower->setValue(audioAnalyzer->getPower(mPower->getSmoothAmnt()));
    
    mPitchFreq->setValue(audioAnalyzer->getPitchFreq(mPitchFreq->getSmoothAmnt()));
    mPitchConf->setValue(audioAnalyzer->getPitchConfidence(mPitchConf->getSmoothAmnt()));
    mSalience->setValue(audioAnalyzer->getMelodySalience(mSalience->getSmoothAmnt()));

    mHfc->setValue(audioAnalyzer->getHfcNormalized(mHfc->getSmoothAmnt()));
    mCentroid->setValue(audioAnalyzer->getCentroidNormalized(mCentroid->getSmoothAmnt()));
    mSpecComp->setValue(audioAnalyzer->getSpectralComplexNormalized(mSpecComp->getSmoothAmnt()));
    mInharm->setValue(audioAnalyzer->getInharmonicity(mInharm->getSmoothAmnt()));

    mOnsets->setValue(audioAnalyzer->getIsOnset());

    mSpectrum->setValues(audioAnalyzer->getSpectrumRef());
    mMelBands->setValues(audioAnalyzer->getMelBandsRef());
    mMfcc->setValues(audioAnalyzer->getDctRef());
    mHpcp->setValues(audioAnalyzer->getHpcpRef());
    
}
//------------------------------------------------
void ofxAAChannelMetersPanel::draw(){
    
    //background
    ofPushStyle();
        ofSetColor(getBackgroundColor());
        ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    for(int i=0; i<meters.size(); i++){
        meters[i]->draw();
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
//------------------------------------------------
void ofxAAChannelMetersPanel::setupMeters(){
    
    VMetersNum = 9; //onsets included
    HMetersNum = 4;
    
    VMetersWidthTotal = _w * 0.75;
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
    mPitchFreq->setMinValue(130.0);//hz
    mPitchFreq->setMaxValue(2093.0);//hz
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
    mSpectrum->setBinsNum(audioAnalyzer->getSpectrumBinsNum());
    mSpectrum->setMinValue(log10(0.001));
    mSpectrum->setMaxValue(log10(1.0));
    meters.push_back(mSpectrum);
    
    y_pos += HMeterHeight;
    mMelBands = new ofxAABinMeter(MTR_NAME_MEL_BANDS, _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mMelBands->setBinsNum(audioAnalyzer->getMelBandsBinsNum());
    mMelBands->setMinValue(log10(0.001));
    mMelBands->setMaxValue(log10(1.0));
    meters.push_back(mMelBands);
    
    y_pos += HMeterHeight;
    mMfcc = new ofxAABinMeter(MTR_NAME_MFCC, _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mMfcc->setBinsNum(audioAnalyzer->getMfccBinsNum());
    mMfcc->setMinValue(0.0);
    mMfcc->setMaxValue(300.0);
    meters.push_back(mMfcc);
    
    y_pos += HMeterHeight;
    mHpcp = new ofxAABinMeter(MTR_NAME_HPCP, _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mHpcp->setBinsNum(audioAnalyzer->getHpcpBinsNum());
    meters.push_back(mHpcp);
    
}

//--------------------------------------------------------------
void ofxAAChannelMetersPanel::onMeterStateChanged(OnOffEventData & data){
    
    //cout << "panel: "<<data.name << endl;
    //cout << "panel: "<<data.state << endl;

    if(data.name == MTR_NAME_POWER){
        audioAnalyzer->setActivePower(data.state);
    }else if(data.name == MTR_NAME_PITCH_FREQ || data.name == MTR_NAME_PITCH_CONF){
        audioAnalyzer->setActivePitch(data.state);
    }else if(data.name == MTR_NAME_PITCH_SALIENCE){
        audioAnalyzer->setActiveMelodySalience(data.state);
    }else if(data.name == MTR_NAME_HFC){
        audioAnalyzer->setActiveHfc(data.state);
    }else if(data.name == MTR_NAME_CENTROID){
        audioAnalyzer->setActiveCentroid(data.state);
    }else if(data.name == MTR_NAME_SPEC_COMP){
        audioAnalyzer->setActiveSpectralComplex(data.state);
    }else if(data.name == MTR_NAME_INHARMONICTY){
        audioAnalyzer->setActiveInharmonicity(data.state);
    }else if(data.name == MTR_NAME_ONSETS){
        audioAnalyzer->setActiveOnsets(data.state);
    }else if(data.name == MTR_NAME_MEL_BANDS || data.name == MTR_NAME_MFCC){
        audioAnalyzer->setActiveMelbandsAndMfcc(data.state);
    }else if(data.name == MTR_NAME_HPCP){
        audioAnalyzer->setActiveHpcp(data.state);
    }
    
}
