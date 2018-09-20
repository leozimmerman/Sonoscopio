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

#include "MeterView.h"
#include "BinMeterView.h"
#include "OnsetMeterView.h"
#include "ofxAAUtils.h"
// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<OnOffEventData> MeterView::onOffEventGlobal = ofEvent<OnOffEventData>();

//FIXME: Move this to somewhere else. Same plase as availableTypes

int MeterView::height = 85;
//------------------------------------------------
MeterView* MeterView::createMeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr){
    switch (algorithmType) {
        case ONSETS: return new OnsetMeterView(algorithmType, panelId, aaPtr);
        case MEL_BANDS: return new BinMeterView(algorithmType, panelId, aaPtr);
        case SPECTRUM: return new BinMeterView(algorithmType, panelId, aaPtr);
        case HPCP: return new BinMeterView(algorithmType, panelId, aaPtr);
        case TRISTIMULUS: return new BinMeterView(algorithmType, panelId, aaPtr);
        case MFCC: return new BinMeterView(algorithmType, panelId, aaPtr);
            
        default:
            return new MeterView(algorithmType, panelId, aaPtr);
    }
}
//------------------------------------------------

#pragma mark Inits
//------------------------------------------------
MeterView::MeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr) {
    
    _audioAnalyzer = aaPtr;
    _algorithmType = algorithmType;
    _name = ofxaa::algorithmTypeToString(algorithmType);
    _panelId = panelId;
    _mainColor.set(ofColor::cyan);
    
    setBackgroundColor(ofColor::darkBlue);//FIXME: Borrar
    
    initDefaultValues();
    initComponents();
    
    setComponentsWidth();
    setComponentsPositions();

}
//------------------------------------------------
void MeterView::initDefaultValues(){
    _value = 0.0;
    _valueNorm = -1.0;
    _minEstimatedValue = 0.0;
    _maxEstimatedValue = 1.0;
    _maxValueRegistered = 0.0;
    _smoothAmnt = 0.0;
    _enabled = TRUE;
    ofColor bordCol = ofColor::grey;
    int bordWidth = 1;
}
//------------------------------------------------
void MeterView::initComponents(){
    font  = new ofTrueTypeFont();
    font->load("gui_assets/fonts/verdana.ttf", 10, false, false);
    font->setLineHeight(14.0f);
    font->setLetterSpacing(1.037);
    _line_h = font->getLineHeight();

    onOffToggle = new OnOffToggle(MTR_ON_OFF, TRUE);
    onOffToggle->onButtonEvent(this, &MeterView::onButtonEvent);
    
    onOffToggle->setHeight(_line_h*0.85);
    onOffToggle->setLabelMargin(0.0);
    onOffToggle->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    onOffToggle->setBackgroundColor(ofColor::black);
    
    smoothSlider = new CustomSlider(MTR_SMOOTHING, 0.0, 1.0, _smoothAmnt);
    smoothSlider->onSliderEvent(this, &MeterView::onSliderEvent);
    smoothSlider->setHeight(_line_h);
    smoothSlider->setLabelMargin(1.0);
    smoothSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    peakButton = new PeakMeterButton(MTR_PEAK);
    peakButton->onButtonEvent(this, &MeterView::onButtonEvent);
    peakButton->setHeight(_line_h*0.85);
    peakButton->setLabelMargin(0.0);
    peakButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
}
//------------------------------------------------
MeterView::~MeterView(){
    delete smoothSlider;
    delete onOffToggle;
    delete peakButton;
}
#pragma mark Update
//------------------------------------------------
void MeterView::update(){
    updateComponents();
    updatePeak();
    updateValues();
}
//------------------------------------------------
void MeterView::updateComponents(){
    smoothSlider->update();
    onOffToggle->update();
    peakButton->update();
}
//------------------------------------------------
void MeterView::updatePeak(){
    if(_value > _maxValueRegistered){
        _maxValueRegistered = _value;
        peakButton->setLabel(ofToString(_maxValueRegistered, 2));
    }
}
//------------------------------------------------
void MeterView::updateValues(){
    setValue(_audioAnalyzer->getValue(_algorithmType, _smoothAmnt));
    setNormalizedValue(_audioAnalyzer->getValue(_algorithmType, _smoothAmnt, TRUE));
}
#pragma mark Draw
//------------------------------------------------
void MeterView::draw(){
    View::draw();
    ofPushStyle();
    drawBounds();
    
    drawLabel();
    onOffToggle->drawTransparent();
    if(_enabled){
        drawValueDisplay();
        drawMeter();
        peakButton->draw();
        smoothSlider->drawSimplified();
    }
    ofPopStyle();
}
//------------------------------------------------
void MeterView::drawBounds(){
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_x, _y, _w, _h);
}
//------------------------------------------------
void MeterView::drawMeter(){
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofFill();
    ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA);//change
    float scaledValue = (_valueNorm >= 0) ? _valueNorm : ofMap(_value, _minEstimatedValue, _maxEstimatedValue, 0.0, 1.0, true);//clamped value
    ofDrawRectangle(0 , 0, _w * scaledValue, _h);
    ofPopMatrix();
}

//------------------------------------------------
void MeterView::drawValueDisplay(){
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofPushStyle();
        ofSetColor(_mainColor);
        string strValue = ofToString(_value, 2);
        int strVal_x = _x + 5;
        font->drawString(strValue, strVal_x, _line_h * 2.0);
    ofPopStyle();
    ofPopMatrix();
}
//------------------------------------------------
void MeterView::drawLabel(){
    ofPushMatrix();
    ofTranslate(_x, _y);
        if(_enabled) ofSetColor(_mainColor);
        else ofSetColor(COLOR_ONOFF_OFF);
        font->drawString(_name, _label_x, _line_h);
        //ofDrawBitmapString(_name, _label_x, _line_h);
    ofPopMatrix();
}

//------------------------------------------------
void MeterView::resetPeak(){
    _maxValueRegistered = 0.0;
    peakButton->setLabel(ofToString(_maxValueRegistered, 2));
}
//------------------------------------------------
#pragma mark - Setters
//------------------------------------------------
void MeterView::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    setComponentsWidth();
    setComponentsPositions();
}
//------------------------------------------------
void MeterView::setComponentsWidth(){
    //-:LABEL
    //constraing width
    float label_w = font->stringWidth(_name);
    float widthForLabel = _w * 0.5;
    if(label_w >= widthForLabel){
        float space_ratio = 1 / (label_w / widthForLabel);
        font->setLetterSpacing(space_ratio);
    } else {
        font->setLetterSpacing(1.37);
    }
    _label_x = _x + 5;
    
    //-:COMPONENTS
    peakButton->setWidth(_w*0.5, 0.0);
    onOffToggle->setWidth(20, 0.0);
    smoothSlider->setWidth(_w*0.5, 0.0);
}
//------------------------------------------------
//add sizes
void MeterView::setComponentsPositions(){

    peakButton->setPosition  (_x + 5, _y + _line_h * 2.2);
    smoothSlider->setPosition(_x + 5, _y + _line_h * 3.2);
    //FIXME: !
    if(_algorithmType != ONSETS){
        onOffToggle->setPosition (_x + _w * 0.1, _y + _line_h * 4.75);
    }else{
        //onsets On-Off goes lower
        onOffToggle->setPosition (_x + _w * 0.1, _y + _line_h * 6.5);
    }
}
//------------------------------------------------
void MeterView::setValue(float val){
    _value = val;
}
//------------------------------------------------
void MeterView::setNormalizedValue(float val){
    _valueNorm = val;
}
//------------------------------------------------
void MeterView::setSmoothAmnt(float val){
    _smoothAmnt = val;
    smoothSlider->setValue(val);
}
//------------------------------------------------
void MeterView::setEnabled(bool state){
    onOffToggle->setEnabled(state);
    _enabled = state;
}
//------------------------------------------------
void MeterView::setMainColor(ofColor col){
    _mainColor = col;
    //colors
    peakButton->setColor(COLOR_PEAKS);
    smoothSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);//back,label,fill
    
    //onOffToggle->setColors(COLOR_ONOFF_ON, COLOR_ONOFF_OFF);
    onOffToggle->setColors(_mainColor, COLOR_ONOFF_OFF);
}
//------------------------------------------------
#pragma mark - Gui listeners
//------------------------------------------------
void MeterView::onSliderEvent(ofxDatGuiSliderEvent e){
    //cout << _name <<"::slider: " <<e.value << endl;
    _smoothAmnt = e.value;
}
//------------------------------------------------
void MeterView::onButtonEvent(ofxDatGuiButtonEvent e){
    
    if(e.target->getLabel()==MTR_ON_OFF){
        OnOffEventData data;
        data.type = _algorithmType;
        data.state = e.enabled;
        data.panelId = _panelId;
        ofNotifyEvent(onOffEventGlobal, data);
        
        _enabled = e.enabled;
     
    } else if(e.target->getType() == ofxDatGuiType::BUTTON){
        resetPeak();
    }

}




