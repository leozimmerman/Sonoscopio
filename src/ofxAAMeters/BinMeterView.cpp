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
#include "StringUtils.h"

int BinMeterView::height = 60;

BinMeterView* BinMeterView::createBinMeterView(ofxAABinsValue valueType, int panelId,  ofxAudioAnalyzerUnit * aaPtr){
    return new BinMeterView(valueType, panelId, aaPtr);
}

BinMeterView::BinMeterView(ofxAABinsValue valueType, int panelId,  ofxAudioAnalyzerUnit * aaPtr) :  MeterView(NONE, panelId, aaPtr){
    
    _binsValueType = valueType;
    _name = utils::binsValueTypeToString(_binsValueType);
    auto binNums = _aaUnit->getValues(_binsValueType).size();
    setBinsNum(binNums);
}

void BinMeterView::setBinsNum(unsigned long bins){
    _nBins = bins;
    _values.resize(_nBins);
}

void BinMeterView::updateValues(){
    setValues(_aaUnit->getValues(_binsValueType, _smoothAmnt, true));
}

void BinMeterView::drawStaticElements(){
    MeterView::drawLabel();
    onOffToggle->drawTransparent();
    if (_enabled) {
        smoothSlider->drawSimplified();
    }
    MeterView::drawBounds();
}

void BinMeterView::drawValueElements(){
    if (_enabled) {
        drawMeter();
    }
}

void BinMeterView::setMaxEstimatedValue(float value){
    _maxEstimatedValue = value;
    _aaUnit->setMaxEstimatedValue(_binsValueType, value);
}

void BinMeterView::drawMeter(){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofPushStyle();
    ofFill();
    ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA);
    float bin_w = (float) _w / _nBins;
    for (int i = 0;i < _values.size(); i++){
        float bin_h = -1 * (_values[i] * _h);
        ofDrawRectangle(i*bin_w, _h, bin_w, bin_h);
    }
    ofPopStyle();
    ofPopMatrix();
}

void BinMeterView::setComponentsWidth(){
    smoothSlider->setWidth(_w * 0.25 , 0.0);
    onOffToggle->setWidth (_w * 0.25, 0.0);
}

void BinMeterView::setComponentsPositions(){
    _label_x = _w * 0.25 + 10;
    onOffToggle->setPosition(_x, _y);
    smoothSlider->setPosition(_x + _w - _w * 0.25, _y);
}

