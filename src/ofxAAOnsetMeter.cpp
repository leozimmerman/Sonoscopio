
#include "ofxAAOnsetMeter.h"

//------------------------------------
ofxAAOnsetMeter::ofxAAOnsetMeter(int x, int y, int w, int h, ofxAudioAnalyzerUnit* analyzerPtr) : ofxAAMeter(MTR_NAME_ONSETS, x, y, w, h){

    audioAnalyzer = analyzerPtr;
    
    _alpha = 0.1;
    alphaSlider = new ofxDatGuiSlider("ALPHA", 0.0, 1.0, _alpha);
    alphaSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    alphaSlider->setWidth(_w * 0.95, 0.0);
    alphaSlider->setHeight(line_h);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    alphaSlider->setPosition(_x + _w*0.05, _y + line_h*2.5);
    
    _treshold = 0.2;
    tresholdSlider = new ofxDatGuiSlider("TRESHOLD", 0.0, 1.0, _treshold);
    tresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    tresholdSlider->setWidth(_w * 0.95, 0.0);
    tresholdSlider->setHeight(line_h);
    tresholdSlider->setLabelMargin(0.0);
    tresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    tresholdSlider->setPosition(_x + _w*0.05, _y + line_h*3.5);
    
    
    
}

//------------------------------------
void ofxAAOnsetMeter::update(){
    
    if(_bDrawFullDisplay==false) return;
    //-----------------------------
        
    onOffToggle->update();
    alphaSlider->update();
    tresholdSlider->update();

}
//------------------------------------
void ofxAAOnsetMeter::draw(){
    
    ofPushStyle();
    
    //bounds-box
    if(_onsetValue)
        ofFill();
    else
        ofNoFill();
    
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    
    //meter----------
    //drawMeter();
    
    //label------------------------------
    if(_onsetValue)
        ofSetColor(_backgroundColor);
    else
        ofSetColor(_mainColor);
    
    drawLabel();
    
    ofPopStyle();
    
    if(_bDrawFullDisplay){
        onOffToggle->draw();
        alphaSlider->drawElemental();
        tresholdSlider->drawElemental();
    }


}
//------------------------------------
void ofxAAOnsetMeter::drawMeter(){
    
    if(getEnabled()==false) return;
    //-----------------------------
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofFill();
    
    //ofSetColor(_backgroundColor);
    
    float meter_h;
    _onsetValue ? meter_h= -1.0 * _h * 0.65 : meter_h=0.0;
    ofDrawRectangle( _w * .25 , _h, _w * .5, meter_h);
    
    ofPopMatrix();
}
//------------------------------------
void ofxAAOnsetMeter::setPosAndSize(int x, int y, int w, int h){
    ofxAAMeter::setPosAndSize(x, y, w, h);
    tresholdSlider->setPosition(_x + _w*0.05, _y + line_h*2.5);
    alphaSlider->setPosition(_x + _w*0.05, _y + line_h*3.5);
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
