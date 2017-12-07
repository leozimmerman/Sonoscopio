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

#include "ofxAAOnsetMeter.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<int> ofxAAOnsetMeter::onsetEventGlobal = ofEvent<int>();

//------------------------------------
ofxAAOnsetMeter::ofxAAOnsetMeter(int x, int y, int w, int h, int panelId, ofxAudioAnalyzerUnit* analyzerPtr) : ofxAAMeter(MTR_NAME_ONSETS, x, y, w, h, panelId){

    //audioAnalyzer = analyzerPtr;
    onsets = analyzerPtr->getOnsetsAlgorithmPtr();
    
    _alpha = onsets->getOnsetAlpha();
    alphaSlider = new CustomSlider(MTR_ALPHA, 0.0, 1.0, _alpha);
    alphaSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    alphaSlider->setHeight(_line_h);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _silenceThreshold = onsets->getOnsetSilenceThreshold();
    silenceThresholdSlider = new CustomSlider(MTR_SIL_TH, 0.0, 1.0, _silenceThreshold);
    silenceThresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    silenceThresholdSlider->setHeight(_line_h);
    silenceThresholdSlider->setLabelMargin(0.0);
    silenceThresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _timeThreshold = onsets->getOnsetTimeThreshold();//ms
    timeThresholdSlider = new CustomSlider(MTR_TI_TH, 0.0, 1000.0, _timeThreshold);
    timeThresholdSlider->onSliderEvent(this, &ofxAAOnsetMeter::onSliderEvent);
    
    timeThresholdSlider->setHeight(_line_h);
    timeThresholdSlider->setLabelMargin(0.0);
    timeThresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _isArmed = false;
    armToggle = new OnOffToggle(MTR_ARM, _isArmed);
    armToggle->onButtonEvent(this, &ofxAAOnsetMeter::onButtonEvent);
    armToggle->setLabels("ARM", "ARM");
    armToggle->setHeight(_line_h*0.85);
    armToggle->setLabelMargin(0.0);
    armToggle->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    armToggle->setBackgroundColor(ofColor::black);
    
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
    armToggle->update();

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
            armToggle->drawTransparent();
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
    
    alphaSlider->setPosition             (_x + _w * 0.1, _y + _line_h * 2.0);
    silenceThresholdSlider->setPosition  (_x + _w * 0.1, _y + _line_h * 3.5);
    timeThresholdSlider->setPosition     (_x + _w * 0.1, _y + _line_h * 5.0);
    armToggle->setPosition               (_x + _w * 0.1, _y + _line_h * 8.0);//On-off at 6.5

}
//------------------------------------------------
void ofxAAOnsetMeter::updateComponentsWidth(){
    
    ofxAAMeter::updateComponentsWidth();
    
    alphaSlider->setWidth(_w*0.8, 0.0);
    silenceThresholdSlider->setWidth(_w*0.8, 0.0);
    timeThresholdSlider->setWidth(_w*0.8, 0.0);
    armToggle->setWidth(_w*0.8, 0.0);
   
}
//------------------------------------------------
void ofxAAOnsetMeter::updateComponentsColors(){
    
    alphaSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    silenceThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    timeThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    armToggle->setColors(ofColor::red, COLOR_ONOFF_OFF);
    
}
//------------------------------------------------
void ofxAAOnsetMeter::setValue(bool val){
    _onsetValue = val;
    if (_onsetValue && _isArmed) {
        sendOnsetEvent();
    }
}
//------------------------------------------------
void ofxAAOnsetMeter::sendOnsetEvent(){
    int value = _panelId;
    
    ofNotifyEvent(onsetEventGlobal, value);
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
    if(e.target->getLabel() == MTR_ALPHA){
        _alpha = e.value;
        onsets->setOnsetAlpha(_alpha);
    }
    else if(e.target->getLabel() == MTR_SIL_TH){
        _silenceThreshold = e.value;
        onsets->setOnsetSilenceThreshold(_silenceThreshold);
    }
    else if(e.target->getLabel() == MTR_TI_TH){
        _timeThreshold = e.value;
        onsets->setOnsetTimeThreshold(_timeThreshold);//make it ms.
    }
}
//------------------------------------------------
void ofxAAOnsetMeter::onButtonEvent(ofxDatGuiButtonEvent e) {
    ofxAAMeter::onButtonEvent(e);
    if(e.target->getLabel()==MTR_ARM){
        _isArmed = e.enabled;
    }
}
