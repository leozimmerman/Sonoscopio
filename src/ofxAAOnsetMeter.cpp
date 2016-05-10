
#include "ofxAAOnsetMeter.h"

//------------------------------------
ofxAAOnsetMeter::ofxAAOnsetMeter(int x, int y, int w, int h, ofxAudioAnalyzer* analyzerPtr) : ofxAAMeter(MTR_NAME_ONSETS, x, y, w, h){

    audioAnalyzer = analyzerPtr;
    
    _alpha = 0.1;
    alphaSlider = new ofxDatGuiSlider("ALPHA", 0.0, 1.0, _alpha);
    alphaSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    alphaSlider->setWidth(_w * 1.3, 0.0);
    alphaSlider->setHeight(line_h);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->setPosition(_x, _y + line_h * 3);
    
    _treshold = 0.2;
    tresholdSlider = new ofxDatGuiSlider("TRESHOLD", 0.0, 1.0, _treshold);
    tresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    tresholdSlider->setWidth(_w * 1.3, 0.0);
    tresholdSlider->setHeight(line_h);
    tresholdSlider->setLabelMargin(0.0);
    tresholdSlider->setPosition(_x, _y + line_h * 4);
    
    
    
}

//------------------------------------
void ofxAAOnsetMeter::update(){
    
    onOffToggle->update();
    alphaSlider->update();
    tresholdSlider->update();

}
//------------------------------------
void ofxAAOnsetMeter::draw(){
    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    
    //meter----------
    drawMeter();
    //label------------------------------
    drawLabel();
    
    ofPopStyle();
    
    onOffToggle->draw();
    alphaSlider->drawElemental();
    tresholdSlider->drawElemental();


}
//------------------------------------
void ofxAAOnsetMeter::drawMeter(){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofFill();
    
    float meter_h;
    _onsetValue ? meter_h= -1.0*_h *0.5 : meter_h=0.0;
    ofDrawRectangle( _w * .25 , _h, _w * .5, meter_h);
    
    ofPopMatrix();
}
//------------------------------------
void ofxAAOnsetMeter::setPosAndSize(int x, int y, int w, int h){
    ofxAAMeter::setPosAndSize(x, y, w, h);
    tresholdSlider->setPosition(_x, _y + line_h * 4);
    alphaSlider->setPosition(_x, _y + line_h * 3);
}
//------------------------------------------------
void ofxAAOnsetMeter::onSliderEvent(ofxDatGuiSliderEvent e){
    if(e.target->getLabel() == "ALPHA"){
        _alpha = e.value;
        audioAnalyzer->setOnsetAlpha(_alpha);
    }else if(e.target->getLabel() == "TRESHOLD"){
        _treshold = e.value;
        audioAnalyzer->setOnsetTreshold(_treshold);
    }
}
