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
#include "ofxAAUtils.h"

int BinMeterView::height = 60;

BinMeterView::BinMeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr) :  MeterView(algorithmType, panelId, aaPtr){
    
    //spectrum cant be turned off
    //mfcc cant work if melBands is turn off
    if(_name == SPECTRUM_STRING || _name == MFCC_STRING){
        onOffToggle->setVisible(false);
        onOffToggle->ofxDatGuiComponent::setEnabled(false);
    }
    int binNums = _audioAnalyzer->getBinsNum(_algorithmType);
    setBinsNum(binNums);
    setMinMaxEstimatedValues();
}

void BinMeterView::updateValues(){
    setValues(_audioAnalyzer->getValues(_algorithmType, _smoothAmnt));
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
            setMinEstimatedValue(0.0);
            setMaxEstimatedValue(1.0);
            break;
    }
}

void BinMeterView::drawMeter(){
    
     ofPushMatrix();
    ofTranslate(_x, _y);
    //ofPushStyle();
    ofFill();
    ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA);
    float bin_w = (float) _w / _nBins;
    for (int i = 0;i < _values.size(); i++){
        float scaledValue = ofMap(_values[i], _minEstimatedValue, _maxEstimatedValue, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * _h);
        ofDrawRectangle(i*bin_w, _h, bin_w, bin_h);
    }
    //ofPopStyle();
    ofPopMatrix();
}

void BinMeterView::setComponentsWidth(){
    MeterView::adjustFontLetterSpacing( _w * 0.5);
    smoothSlider->setWidth(_w * 0.25 , 0.0);
    onOffToggle->setWidth (_w * 0.25, 0.0);
}

void BinMeterView::setComponentsPositions(){
    _label_x =  _w * .5 - _label_w *.5; //align center
    onOffToggle->setPosition(_x, _y);
    smoothSlider->setPosition(_x + _w - _w * 0.25, _y);
}

