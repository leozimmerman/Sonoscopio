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
#include "GuiFactory.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<int> OnsetMeterView::onsetEventGlobal = ofEvent<int>();

int OnsetMeterView::height = 100;

OnsetMeterView::OnsetMeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr) : MeterView(algorithmType, panelId, aaPtr){
    
    onsets = aaPtr->getOnsetsAlgorithmPtr();
    
    _alpha = onsets->getOnsetAlpha();
    alphaSlider = GuiFactory::createCustomSlider(MTR_ALPHA, 0.0, 1.0, _alpha, this, &OnsetMeterView::onSliderEvent);
    
    _silenceThreshold = onsets->getOnsetSilenceThreshold();
    silenceThresholdSlider = GuiFactory::createCustomSlider(MTR_SIL_TH, 0.0, 1.0, _silenceThreshold, this, &OnsetMeterView::onSliderEvent);
    
    _timeThreshold = onsets->getOnsetTimeThreshold();//ms
    timeThresholdSlider = GuiFactory::createCustomSlider(MTR_TI_TH, 0.0, 1000.0, _timeThreshold, this, &OnsetMeterView::onSliderEvent);
    
    _isArmed = false;
    armToggle = GuiFactory::createOnOffToggle(MTR_ARM, _isArmed, this, &OnsetMeterView::onButtonEvent);
    armToggle->setLabels(MTR_ARM, MTR_ARM);
    
    resetButton = GuiFactory::createTransparentMeterButton(MTR_RESET, this, &OnsetMeterView::onButtonEvent);
    resetButton->setColor(ofColor::white);
    
    setComponentsHeight();
    setComponentsWidth();
    setComponentsPositions();
}

OnsetMeterView::~OnsetMeterView(){
    delete alphaSlider;
    delete silenceThresholdSlider;
    delete timeThresholdSlider;
    delete armToggle;
    delete resetButton;
}

void OnsetMeterView::update(){
    onOffToggle->update();
    alphaSlider->update();
    silenceThresholdSlider->update();
    timeThresholdSlider->update();
    armToggle->update();
    resetButton->update();

    setValue(_audioAnalyzer->getOnsetValue());
}

void OnsetMeterView::drawStaticElements(){
    drawLabel();
    onOffToggle->drawTransparent();
    if (_enabled){
        alphaSlider->drawSimplified();
        silenceThresholdSlider->drawSimplified();
        timeThresholdSlider->drawSimplified();
        armToggle->drawTransparent();
        resetButton->drawTransparent();
    }
    drawBounds();
}

void OnsetMeterView::drawValueElements(){
    if (_enabled){
        drawMeter();
    }
}

void OnsetMeterView::drawMeter(){
    if (_onsetValue){
        ofPushMatrix();
        ofTranslate(_x, _y);
        ofPushStyle();
        ofFill();
        ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA * 2.0);
            ofDrawRectangle(0 , 0, _w, _h);
        ofPopStyle();
        ofPopMatrix();
    }
}

void OnsetMeterView::setComponentsWidth(){
    MeterView::setComponentsWidth();
    alphaSlider->setWidth(_w*0.8, 0.0);
    silenceThresholdSlider->setWidth(_w*0.8, 0.0);
    timeThresholdSlider->setWidth(_w*0.8, 0.0);
    armToggle->setWidth(_w*0.8, 0.0);
    resetButton->setWidth(_w*0.25, 0.0);
}

void OnsetMeterView::setComponentsHeight(){
    alphaSlider->setHeight(_line_h);
    silenceThresholdSlider->setHeight(_line_h);
    timeThresholdSlider->setHeight(_line_h);
    armToggle->setHeight(_line_h);
    resetButton->setHeight(_line_h);
}

void OnsetMeterView::setComponentsPositions(){
    MeterView::setComponentsPositions();
    alphaSlider->setPosition             (_x + 5, _y + _line_h * 2.0);
    silenceThresholdSlider->setPosition  (_x + 5, _y + _line_h * 3.5);
    timeThresholdSlider->setPosition     (_x + 5, _y + _line_h * 5.0);
    armToggle->setPosition               (_x + 5, _y + _line_h * 6.5);
    resetButton->setPosition             (_x + _w - _w * 0.25, _y);
}

void OnsetMeterView::updateComponentsColors(){
    alphaSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    silenceThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    timeThresholdSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);
    armToggle->setColors(ofColor::red, COLOR_ONOFF_OFF);
}

void OnsetMeterView::setValue(bool val){
    _onsetValue = val;
    if (_onsetValue && _isArmed) {
        sendOnsetEvent();
    }
}

void OnsetMeterView::sendOnsetEvent(){
    int value = _panelId;
    ofNotifyEvent(onsetEventGlobal, value);
}

void OnsetMeterView::setAlpha(float alpha){
    _alpha = alpha;
    alphaSlider->setValue(alpha);
    if(onsets!=NULL)
        onsets->setOnsetAlpha(alpha);
}

void OnsetMeterView::setSilenceThreshold(float tres){
    _silenceThreshold = tres;
    silenceThresholdSlider->setValue(tres);
    if(onsets!=NULL)
        onsets->setOnsetSilenceThreshold(tres);
}

void OnsetMeterView::setTimeThreshold(float tres){
    _timeThreshold = tres;
    timeThresholdSlider->setValue(tres);
    if(onsets!=NULL){
        onsets->setOnsetTimeThreshold(tres);
    }
}

void OnsetMeterView::onSliderEvent(ofxDatGuiSliderEvent e){
    if (e.target->getLabel() == MTR_ALPHA){
        _alpha = e.value;
        onsets->setOnsetAlpha(_alpha);
    }
    else if (e.target->getLabel() == MTR_SIL_TH){
        _silenceThreshold = e.value;
        onsets->setOnsetSilenceThreshold(_silenceThreshold);
    }
    else if (e.target->getLabel() == MTR_TI_TH){
        _timeThreshold = e.value;
        onsets->setOnsetTimeThreshold(_timeThreshold);//make it ms.
    }
}

void OnsetMeterView::onButtonEvent(ofxDatGuiButtonEvent e) {
    MeterView::onButtonEvent(e);
    if (e.target->getLabel() == MTR_ARM){
        _isArmed = e.enabled;
    } else if(e.target->getLabel() == MTR_RESET){
        onsets->reset();
    }
}
