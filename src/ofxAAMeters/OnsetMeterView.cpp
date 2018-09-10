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

#include "OnsetMeterView.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<int> OnsetMeterView::onsetEventGlobal = ofEvent<int>();

//------------------------------------
OnsetMeterView::OnsetMeterView(int x, int y, int w, int h, int panelId, ofxAudioAnalyzerUnit* analyzerPtr) : MeterView(MTR_NAME_ONSETS, x, y, w, h, panelId){

    //audioAnalyzer = analyzerPtr;
    onsets = analyzerPtr->getOnsetsAlgorithmPtr();
    
    _alpha = onsets->getOnsetAlpha();
    alphaSlider = new CustomSlider(MTR_ALPHA, 0.0, 1.0, _alpha);
    alphaSlider->onSliderEvent(this, &OnsetMeterView::onSliderEvent);
    
    alphaSlider->setHeight(_line_h);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _silenceThreshold = onsets->getOnsetSilenceThreshold();
    silenceThresholdSlider = new CustomSlider(MTR_SIL_TH, 0.0, 1.0, _silenceThreshold);
    silenceThresholdSlider->onSliderEvent(this, &OnsetMeterView::onSliderEvent);
    
    silenceThresholdSlider->setHeight(_line_h);
    silenceThresholdSlider->setLabelMargin(0.0);
    silenceThresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _timeThreshold = onsets->getOnsetTimeThreshold();//ms
    timeThresholdSlider = new CustomSlider(MTR_TI_TH, 0.0, 1000.0, _timeThreshold);
    timeThresholdSlider->onSliderEvent(this, &OnsetMeterView::onSliderEvent);
    
    timeThresholdSlider->setHeight(_line_h);
    timeThresholdSlider->setLabelMargin(0.0);
    timeThresholdSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _isArmed = false;
    armToggle = new OnOffToggle(MTR_ARM, _isArmed);
    armToggle->onButtonEvent(this, &OnsetMeterView::onButtonEvent);
    armToggle->setLabels("ARM", "ARM");
    armToggle->setHeight(_line_h*0.85);
    armToggle->setLabelMargin(0.0);
    armToggle->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    armToggle->setBackgroundColor(ofColor::black);
    
    updateComponentsWidth();
    updateComponentsPositions();
}

//------------------------------------
void OnsetMeterView::update(){
    
    if(_bDrawFullDisplay==false) return;
    //-----------------------------
        
    onOffToggle->update();
    alphaSlider->update();
    silenceThresholdSlider->update();
    timeThresholdSlider->update();
    armToggle->update();

}
//------------------------------------
void OnsetMeterView::draw(){
    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    ofPopStyle();
    
    return;
    //meter----------
    
    drawLabel();
    
    if(_enabled)
        drawMeter();
    
    
    
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
void OnsetMeterView::drawMeter(){
    
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
void OnsetMeterView::resize(int x, int y, int w, int h){
    
    MeterView::resize(x, y, w, h);
    
    updateComponentsWidth();
    updateComponentsPositions();
    
}
//------------------------------------------------
void OnsetMeterView::updateComponentsPositions(){
    
    MeterView::updateComponentsPositions();
    
    alphaSlider->setPosition             (_x + _w * 0.1, _y + _line_h * 2.0);
    silenceThresholdSlider->setPosition  (_x + _w * 0.1, _y + _line_h * 3.5);
    timeThresholdSlider->setPosition     (_x + _w * 0.1, _y + _line_h * 5.0);
    armToggle->setPosition               (_x + _w * 0.1, _y + _line_h * 8.0);//On-off at 6.5

}
//------------------------------------------------
void OnsetMeterView::updateComponentsWidth(){
    
    MeterView::updateComponentsWidth();
    
    alphaSlider->setWidth(_w*0.8, 0.0);
    silenceThresholdSlider->setWidth(_w*0.8, 0.0);
    timeThresholdSlider->setWidth(_w*0.8, 0.0);
    armToggle->setWidth(_w*0.8, 0.0);
   
}
//------------------------------------------------
void OnsetMeterView::updateComponentsColors(){
    
    alphaSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    silenceThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    timeThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    armToggle->setColors(ofColor::red, COLOR_ONOFF_OFF);
    
}
//------------------------------------------------
void OnsetMeterView::setValue(bool val){
    _onsetValue = val;
    if (_onsetValue && _isArmed) {
        sendOnsetEvent();
    }
}
//------------------------------------------------
void OnsetMeterView::sendOnsetEvent(){
    int value = _panelId;
    
    ofNotifyEvent(onsetEventGlobal, value);
}
//------------------------------------------------
void OnsetMeterView::setAlpha(float alpha){
    _alpha = alpha;
    alphaSlider->setValue(alpha);
    if(onsets!=NULL)
        onsets->setOnsetAlpha(alpha);
}
//------------------------------------------------
void OnsetMeterView::setSilenceThreshold(float tres){
    _silenceThreshold = tres;
    silenceThresholdSlider->setValue(tres);
    if(onsets!=NULL)
        onsets->setOnsetSilenceThreshold(tres);
}
//------------------------------------------------
void OnsetMeterView::setTimeThreshold(float tres){
    _timeThreshold = tres;
    timeThresholdSlider->setValue(tres);
    if(onsets!=NULL){
        onsets->setOnsetTimeThreshold(tres);
    }
}
//------------------------------------------------
void OnsetMeterView::onSliderEvent(ofxDatGuiSliderEvent e){
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
void OnsetMeterView::onButtonEvent(ofxDatGuiButtonEvent e) {
    MeterView::onButtonEvent(e);
    if(e.target->getLabel()==MTR_ARM){
        _isArmed = e.enabled;
    }
}
