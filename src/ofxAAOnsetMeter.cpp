
#include "ofxAAOnsetMeter.h"

//------------------------------------
ofxAAOnsetMeter::ofxAAOnsetMeter(int x, int y, int w, int h, ofxAudioAnalyzerUnit* analyzerPtr) : ofxAAMeter(MTR_NAME_ONSETS, x, y, w, h){

    //audioAnalyzer = analyzerPtr;
    onsets = analyzerPtr->getOnsetsAlgorithmPtr();
    
    _alpha = onsets->getOnsetAlpha();
    alphaSlider = new CustomSlider("ALPHA", 0.0, 1.0, _alpha);
    alphaSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    alphaSlider->setHeight(_line_h);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _silenceTreshold = onsets->getOnsetSilenceTreshold();
    silenceTresholdSlider = new CustomSlider("SIL-TH", 0.0, 1.0, _silenceTreshold);
    silenceTresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    silenceTresholdSlider->setHeight(_line_h);
    silenceTresholdSlider->setLabelMargin(0.0);
    silenceTresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _timeTreshold = onsets->getOnsetTimeTreshold();//ms
    timeTresholdSlider = new CustomSlider("TI-TH", 0.0, 1000.0, _timeTreshold);
    timeTresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    timeTresholdSlider->setHeight(_line_h);
    timeTresholdSlider->setLabelMargin(0.0);
    timeTresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    updateComponentsWidth();
    updateComponentsPositions();
}

//------------------------------------
void ofxAAOnsetMeter::update(){
    
    if(_bDrawFullDisplay==false) return;
    //-----------------------------
        
    onOffToggle->update();
    alphaSlider->update();
    silenceTresholdSlider->update();
    timeTresholdSlider->update();

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
            silenceTresholdSlider->drawSimplified();
            timeTresholdSlider->drawSimplified();
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
    silenceTresholdSlider->setPosition  (_x + _w * 0.1, _y + _line_h * 3.5);
    timeTresholdSlider->setPosition     (_x + _w * 0.1, _y + _line_h * 5.0);

}
//------------------------------------------------
void ofxAAOnsetMeter::updateComponentsWidth(){
    
    ofxAAMeter::updateComponentsWidth();
    
    alphaSlider->setWidth(_w*0.8, 0.0);
    silenceTresholdSlider->setWidth(_w*0.8, 0.0);
    timeTresholdSlider->setWidth(_w*0.8, 0.0);
   
}
//------------------------------------------------
void ofxAAOnsetMeter::updateSlidersColors(){
    
    alphaSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    silenceTresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    timeTresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    
}
//------------------------------------------------
void ofxAAOnsetMeter::setAlpha(float alpha){
    _alpha = alpha;
    alphaSlider->setValue(alpha);
    if(onsets!=NULL)
        onsets->setOnsetAlpha(alpha);
}
//------------------------------------------------
void ofxAAOnsetMeter::setSilenceTreshold(float tres){
    _silenceTreshold = tres;
    silenceTresholdSlider->setValue(tres);
    if(onsets!=NULL)
        onsets->setOnsetSilenceTreshold(tres);
}
//------------------------------------------------
void ofxAAOnsetMeter::setTimeTreshold(float tres){
    _timeTreshold = tres;
    timeTresholdSlider->setValue(tres);
    if(onsets!=NULL){
        onsets->setOnsetTimeTreshold(tres);
    }
}
//------------------------------------------------
void ofxAAOnsetMeter::onSliderEvent(ofxDatGuiSliderEvent e){
    if(e.target->getLabel() == "ALPHA"){
        _alpha = e.value;
        onsets->setOnsetAlpha(_alpha);
    }
    else if(e.target->getLabel() == "SIL-TH"){
        _silenceTreshold = e.value;
        onsets->setOnsetSilenceTreshold(_silenceTreshold);
    }
    else if(e.target->getLabel() == "TI-TH"){
        _timeTreshold = e.value;
        onsets->setOnsetTimeTreshold(_timeTreshold);//make it ms.
    }
}
