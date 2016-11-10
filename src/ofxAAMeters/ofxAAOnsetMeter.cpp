
#include "ofxAAOnsetMeter.h"

//------------------------------------
ofxAAOnsetMeter::ofxAAOnsetMeter(int x, int y, int w, int h, int panelId, ofxAudioAnalyzerUnit* analyzerPtr) : ofxAAMeter(MTR_NAME_ONSETS, x, y, w, h, panelId){

    //audioAnalyzer = analyzerPtr;
    onsets = analyzerPtr->getOnsetsAlgorithmPtr();
    
    _alpha = onsets->getOnsetAlpha();
    alphaSlider = new CustomSlider("ALPHA", 0.0, 1.0, _alpha);
    alphaSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    alphaSlider->setHeight(_line_h);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _silenceThreshold = onsets->getOnsetSilenceThreshold();
    silenceThresholdSlider = new CustomSlider("SIL-TH", 0.0, 1.0, _silenceThreshold);
    silenceThresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    silenceThresholdSlider->setHeight(_line_h);
    silenceThresholdSlider->setLabelMargin(0.0);
    silenceThresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _timeThreshold = onsets->getOnsetTimeThreshold();//ms
    timeThresholdSlider = new CustomSlider("TI-TH", 0.0, 1000.0, _timeThreshold);
    timeThresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    timeThresholdSlider->setHeight(_line_h);
    timeThresholdSlider->setLabelMargin(0.0);
    timeThresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    updateComponentsWidth();
    updateComponentsPositions();
}

//------------------------------------
void ofxAAOnsetMeter::update(){
    
    if(_bDrawFullDisplay==false) return;
    //-----------------------------
        
    onOffToggle->update();
    alphaSlider->update();
    silenceThresholdSlider->update();
    timeThresholdSlider->update();

}
//------------------------------------
void ofxAAOnsetMeter::draw(){
    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    
    //meter----------
    
    drawLabel();
    
    if(_enabled)
        drawMeter();
    
    ofPopStyle();
    
    if(_bDrawFullDisplay){
        
        onOffToggle->drawTransparent();
        if(_enabled){
            alphaSlider->drawSimplified();
            silenceThresholdSlider->drawSimplified();
            timeThresholdSlider->drawSimplified();
        }
    }

}
//------------------------------------
void ofxAAOnsetMeter::drawMeter(){
    
    if(getEnabled()==false) return;
    //-----------------------------
 
    if(_onsetValue){
        
        ofPushStyle();
        ofFill();
        ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA * 2.0);
        ofDrawRectangle(_drawRect);
        ofPopStyle();
    }
    
}
//------------------------------------
void ofxAAOnsetMeter::setPosAndSize(int x, int y, int w, int h){
    
    ofxAAMeter::setPosAndSize(x, y, w, h);
    
    updateComponentsWidth();
    updateComponentsPositions();
    
}
//------------------------------------------------
void ofxAAOnsetMeter::updateComponentsPositions(){
    
    ofxAAMeter::updateComponentsPositions();
    
    alphaSlider->setPosition            (_x + _w * 0.1, _y + _line_h * 2.0);
    silenceThresholdSlider->setPosition  (_x + _w * 0.1, _y + _line_h * 3.5);
    timeThresholdSlider->setPosition     (_x + _w * 0.1, _y + _line_h * 5.0);

}
//------------------------------------------------
void ofxAAOnsetMeter::updateComponentsWidth(){
    
    ofxAAMeter::updateComponentsWidth();
    
    alphaSlider->setWidth(_w*0.8, 0.0);
    silenceThresholdSlider->setWidth(_w*0.8, 0.0);
    timeThresholdSlider->setWidth(_w*0.8, 0.0);
   
}
//------------------------------------------------
void ofxAAOnsetMeter::updateSlidersColors(){
    
    alphaSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    silenceThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    timeThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    
}
//------------------------------------------------
void ofxAAOnsetMeter::setAlpha(float alpha){
    _alpha = alpha;
    alphaSlider->setValue(alpha);
    if(onsets!=NULL)
        onsets->setOnsetAlpha(alpha);
}
//------------------------------------------------
void ofxAAOnsetMeter::setSilenceThreshold(float tres){
    _silenceThreshold = tres;
    silenceThresholdSlider->setValue(tres);
    if(onsets!=NULL)
        onsets->setOnsetSilenceThreshold(tres);
}
//------------------------------------------------
void ofxAAOnsetMeter::setTimeThreshold(float tres){
    _timeThreshold = tres;
    timeThresholdSlider->setValue(tres);
    if(onsets!=NULL){
        onsets->setOnsetTimeThreshold(tres);
    }
}
//------------------------------------------------
void ofxAAOnsetMeter::onSliderEvent(ofxDatGuiSliderEvent e){
    if(e.target->getLabel() == "ALPHA"){
        _alpha = e.value;
        onsets->setOnsetAlpha(_alpha);
    }
    else if(e.target->getLabel() == "SIL-TH"){
        _silenceThreshold = e.value;
        onsets->setOnsetSilenceThreshold(_silenceThreshold);
    }
    else if(e.target->getLabel() == "TI-TH"){
        _timeThreshold = e.value;
        onsets->setOnsetTimeThreshold(_timeThreshold);//make it ms.
    }
}
