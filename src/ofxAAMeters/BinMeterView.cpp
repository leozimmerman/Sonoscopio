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

#include "BinMeterView.h"

BinMeterView::BinMeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr) :  MeterView(algorithmType, panelId, aaPtr){
    
    setBackgroundColor(ofColor::pink);
    
    smoothSlider->setHeight(_line_h*0.75);
    onOffToggle->setHeight(_line_h*0.75);
    
    updateComponentsPositions();
    updateComponentsWidth();
    
    //spectrum cant be turned off
    //mfcc cant work if melBands is turn off
    if(_name == MTR_NAME_SPECTRUM || _name == MTR_NAME_MFCC){
        onOffToggle->setVisible(false);
        onOffToggle->ofxDatGuiComponent::setEnabled(false);
    }
    
    setBinsNum(_audioAnalyzer->getBinsNum(_algorithmType));
    setMinMaxEstimatedValues();
}

//-------------------------------------------------------
void BinMeterView::draw(){
    //bounds-box
    ofPushStyle();
    ofNoFill();
    ofSetColor(_mainColor);
    
    ofPopStyle();

    return;
    
    drawLabel();
    
    
    if(_enabled) drawMeter();
    
    
    onOffToggle->drawTransparent();
    if(_enabled) smoothSlider->drawSimplified();
    
    

}
//-------------------------------------------------------
void BinMeterView::setMinMaxEstimatedValues() {
    switch (_algorithmType) {
        case SPECTRUM:
            setMinEstimatedValue(DB_MIN);
            setMaxEstimatedValue(DB_MAX);
            break;
        case MEL_BANDS:
            setMinEstimatedValue(DB_MIN);
            setMaxEstimatedValue(DB_MAX);
            break;
        case MFCC:
            setMinEstimatedValue(0.0);
            setMaxEstimatedValue(MFCC_MAX_ESTIMATED_VALUE);
            break;
        default:
            break;
    }
}
//-------------------------------------------------------
void BinMeterView::drawMeter(){
    
    if(getEnabled()==false) return;
    //-----------------------------
    
    ofFill();
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofPushStyle();
    ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA);
    
    
    float bin_w = (float) _w / _nBins;
    
    for (int i = 0;i < _values.size(); i++){
        float scaledValue = ofMap(_values[i], _minEstimatedValue, _maxEstimatedValue, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * _h);
        ofDrawRectangle(i*bin_w, _h, bin_w, bin_h);
    }
    
    ofPopStyle();
    
    ofPopMatrix();


}
//-------------------------------------------------------
void BinMeterView::drawLabel(){
    ofPushMatrix();
    ofTranslate(_x, _y);
    if(_enabled) ofSetColor(_mainColor);
    else ofSetColor(COLOR_ONOFF_OFF);
    font->drawString(_name, _label_x, _line_h * 0.75);
    ofPopMatrix();
}


//-------------------------------------------------------
void BinMeterView::resize(int x, int y, int w, int h){
    
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    ///_drawRect.set(x, y, w, h);
    
    updateComponentsPositions();
    updateComponentsWidth();
    //updateColors();
    
}
//-------------------------------------------------------
void BinMeterView::updateComponentsPositions(){
   
    smoothSlider->setPosition(_x + _w - _w * 0.25, _y);
    onOffToggle->setPosition(_x, _y);
    
}
//-------------------------------------------------------
void BinMeterView::updateComponentsWidth(){
    
    //-:LABEL
    //constraing width
    float label_w = font->stringWidth(_name);
    float widthForLabel = _w * 0.95;
    if(label_w >= widthForLabel){
        float space_ratio = 1 / (label_w / widthForLabel);
        font->setLetterSpacing(space_ratio);
    }
    //align center
    label_w = font->stringWidth(_name);
    _label_x =  _w * .5 - label_w *.5;
    
    smoothSlider->setWidth(_w * 0.25 , 0.0);
    onOffToggle->setWidth (_w * 0.25, 0.0);

}
