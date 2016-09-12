
#include "ofxAAOnsetMeter.h"

//------------------------------------
ofxAAOnsetMeter::ofxAAOnsetMeter(int x, int y, int w, int h, ofxAudioAnalyzerUnit* analyzerPtr) : ofxAAMeter(MTR_NAME_ONSETS, x, y, w, h){

    //audioAnalyzer = analyzerPtr;
    onsets = analyzerPtr->getOnsetsAlgorithmPtr();
    
    _alpha = onsets->getOnsetAlpha();
    alphaSlider = new ofxDatGuiSlider("ALPHA", 0.0, 1.0, _alpha);
    alphaSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    alphaSlider->setWidth(_w * 0.95, 0.0);
    alphaSlider->setHeight(line_h);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    alphaSlider->setPosition(_x + _w*0.05, _y + line_h*2.5);
    
    _silenceTreshold = onsets->getOnsetSilenceTreshold();
    silenceTresholdSlider = new ofxDatGuiSlider("SLNC-TRES", 0.0, 1.0, _silenceTreshold);
    silenceTresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    silenceTresholdSlider->setWidth(_w * 0.95, 0.0);
    silenceTresholdSlider->setHeight(line_h);
    silenceTresholdSlider->setLabelMargin(0.0);
    silenceTresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    silenceTresholdSlider->setPosition(_x + _w*0.05, _y + line_h*4);
    
    _timeTreshold = onsets->getOnsetTimeTreshold();//ms
    timeTresholdSlider = new ofxDatGuiSlider("TIME-TRES", 0.0, 1000.0, _timeTreshold);
    timeTresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    timeTresholdSlider->setWidth(_w * 0.95, 0.0);
    timeTresholdSlider->setHeight(line_h);
    timeTresholdSlider->setLabelMargin(0.0);
    timeTresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    timeTresholdSlider->setPosition(_x + _w*0.05, _y + line_h*5.5);
    
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
        silenceTresholdSlider->drawElemental();
        timeTresholdSlider->drawElemental();
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
    alphaSlider->setPosition(_x + _w*0.05, _y + line_h*2.5);
    silenceTresholdSlider->setPosition(_x + _w*0.05, _y + line_h*4.0);
    timeTresholdSlider->setPosition(_x + _w*0.05, _y + line_h*5.5);
    
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
    if(onsets!=NULL)
        onsets->setOnsetTimeTreshold(tres);
}
//------------------------------------------------
void ofxAAOnsetMeter::onSliderEvent(ofxDatGuiSliderEvent e){
    if(e.target->getLabel() == "ALPHA"){
        _alpha = e.value;
        onsets->setOnsetAlpha(_alpha);
    }
    else if(e.target->getLabel() == "SLNC-TRES"){
        _silenceTreshold = e.value;
        onsets->setOnsetSilenceTreshold(_silenceTreshold);
    }
    else if(e.target->getLabel() == "TIME-TRES"){
        _timeTreshold = e.value;
        onsets->setOnsetTimeTreshold(_timeTreshold);
    }
}
