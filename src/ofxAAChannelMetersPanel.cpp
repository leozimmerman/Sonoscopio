
#include "ofxAAChannelMetersPanel.h"

void ofxAAChannelMetersPanel::setup(int x, int y, int width, int height, ofxAudioAnalyzer * aaPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    setBackgroundColor(ofColor::darkorange);
    audioAnalyzerPtr = aaPtr;
    
    
 
    
    //ADD-METERS-------------------------------------------
    setupMeters();

    
}
//------------------------------------------------
void ofxAAChannelMetersPanel::update(){
    
    for(int i=0; i<meters.size(); i++){
        meters[i]->update();
    }
    
  
    //------------------------
    power->setValue(audioAnalyzerPtr->getPower());
    
    pitchFreq->setValue(audioAnalyzerPtr->getPitchFreq());
    pitchConf->setValue(audioAnalyzerPtr->getPitchConf());
    salience->setValue(audioAnalyzerPtr->getSalience());
    
    hfc->setValue(audioAnalyzerPtr->getHfcNorm());
    centroid->setValue(audioAnalyzerPtr->getCentroidNorm());
    specComp->setValue(audioAnalyzerPtr->getSpectralComplexNorm());
    inharm->setValue(audioAnalyzerPtr->getInharmonicity());
    
    onsets->setValue(audioAnalyzerPtr->getIsOnset());
    
    
    
    //spectrum->setNumBins(audioAnalyzerPtr->getSpectrumBinsNum());
    spectrum->setValues(audioAnalyzerPtr->getSpectrumRef());
    melBands->setValues(audioAnalyzerPtr->getMelBandsRef());
    mfcc->setValues(audioAnalyzerPtr->getDctRef());
    hpcp->setValues(audioAnalyzerPtr->getHpcpRef());
    
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
    
    //power->setPosAndSize(_x+30, _y, 50, _h);
    
    
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
    power = new ofxAAMeter("POWER", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(power);
    
    x_pos += VMeterWidth;
    pitchFreq = new ofxAAMeter("PITCH FREQ", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    pitchFreq->setMinValue(130.0);//hz
    pitchFreq->setMaxValue(2093.0);//hz
    meters.push_back(pitchFreq);
    
    x_pos += VMeterWidth;
    pitchConf = new ofxAAMeter("PITCH CONF", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(pitchConf);
    
    x_pos += VMeterWidth;
    salience = new ofxAAMeter("PITCH SALIENCE", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(salience);
    
    x_pos += VMeterWidth;
    hfc = new ofxAAMeter("HFC", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(hfc);
    
    x_pos += VMeterWidth;
    centroid = new ofxAAMeter("CENTROID", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(centroid);
    
    x_pos += VMeterWidth;
    specComp = new ofxAAMeter("SPEC COMP", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(specComp);
    
    x_pos += VMeterWidth;
    inharm = new ofxAAMeter("INHARM", _x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(inharm);
    
    x_pos += VMeterWidth;
    onsets = new ofxAAOnsetMeter(_x + x_pos, _y, VMeterWidth, VMeterHeight);
    meters.push_back(onsets);
    
    //Horizontal Meters
    x_pos = VMetersWidthTotal;
    int y_pos = 0;
    spectrum = new ofxAABinMeter("SPECTRUM", _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    spectrum->setBinsNum(audioAnalyzerPtr->getSpectrumBinsNum());
    spectrum->setMinValue(log10(0.001));
    spectrum->setMaxValue(log10(1.0));
    meters.push_back(spectrum);
    
    y_pos += HMeterHeight;
    melBands = new ofxAABinMeter("MEL BANDS", _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    melBands->setBinsNum(audioAnalyzerPtr->getMelBandsBinsNum());
    melBands->setMinValue(log10(0.001));
    melBands->setMaxValue(log10(1.0));
    meters.push_back(melBands);
    
    y_pos += HMeterHeight;
    mfcc = new ofxAABinMeter("MFCC", _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    mfcc->setBinsNum(audioAnalyzerPtr->getMfccBinsNum());
    mfcc->setMinValue(0.0);
    mfcc->setMaxValue(300.0);
    meters.push_back(mfcc);
    
    y_pos += HMeterHeight;
    hpcp = new ofxAABinMeter("HPCP", _x + x_pos, _y + y_pos, HMeterWidth, HMeterHeight);
    hpcp->setBinsNum(audioAnalyzerPtr->getHpcpBinsNum());
    meters.push_back(hpcp);
    
 

}
