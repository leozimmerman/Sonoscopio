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

#include "ofxAAMeter.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<OnOffEventData> ofxAAMeter::onOffEventGlobal = ofEvent<OnOffEventData>();

//------------------------------------------------
#pragma mark - Core funcs
//------------------------------------------------
ofxAAMeter::ofxAAMeter(string name, int x, int y, int w, int h, int panelId){
    
    _name = name;
    
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    _panelId = panelId;
    
    _drawRect.set(x, y, w, h);
    
    _backgroundColor.set(ofColor::black);
    _mainColor.set(ofColor::white);
    
    _value = 0.0;
    _valueNorm = -1.0;
    _minEstimatedValue = 0.0;
    _maxEstimatedValue = 1.0;
    _maxValueRegistered = 0.0;
    
    _smoothAmnt = 0.0;
    
    _bDrawFullDisplay = TRUE;
    _enabled = TRUE;
    
    ofColor bordCol = ofColor::grey;
    int bordWidth = 1;
    
    //Font-Label----------
    verdana  = new ofTrueTypeFont();
    verdana->load("gui_assets/fonts/verdana.ttf", 10, false, false);
    verdana->setLineHeight(14.0f);
    verdana->setLetterSpacing(1.037);
    _line_h = verdana->getLineHeight();
    
    //constraing width
    float label_w = verdana->stringWidth(_name);
    float widthForLabel = _w * 0.95;
    if(label_w >= widthForLabel){
        float space_ratio = 1 / (label_w / widthForLabel);
        verdana->setLetterSpacing(space_ratio);
    }
    //align center
    label_w = verdana->stringWidth(_name);
    _label_x =  _w * .5 - label_w *.5;
    
    //-----------------
    
//    onOffToggle = new ofxDatGuiToggle(MTR_ON_OFF, TRUE);
    onOffToggle = new OnOffToggle(MTR_ON_OFF, TRUE);
    onOffToggle->onButtonEvent(this, &ofxAAMeter::onButtonEvent);
    
    onOffToggle->setHeight(_line_h*0.85);
    onOffToggle->setLabelMargin(0.0);
    onOffToggle->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    onOffToggle->setBackgroundColor(ofColor::black);
    
    smoothSlider = new CustomSlider(MTR_SMOOTHING, 0.0, 1.0, _smoothAmnt);
    smoothSlider->onSliderEvent(this, &ofxAAMeter::onSliderEvent);
    smoothSlider->setHeight(_line_h);
    smoothSlider->setLabelMargin(1.0);
    smoothSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    peakButton = new PeakMeterButton(MTR_PEAK);
    peakButton->onButtonEvent(this, &ofxAAMeter::onButtonEvent);
    peakButton->setHeight(_line_h*0.85);
    peakButton->setLabelMargin(0.0);
    peakButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    
    _meterOrient = VERTICAL;
    
    updateComponentsWidth();
    updateComponentsPositions();

}
//------------------------------------------------
ofxAAMeter::~ofxAAMeter(){
    
    delete smoothSlider;
    delete onOffToggle;
    delete peakButton;
    
}
//------------------------------------------------
void ofxAAMeter::update(){
    
     if(_bDrawFullDisplay==false) return;
    //-----------------------------
    
    smoothSlider->update();
    onOffToggle->update();
    peakButton->update();
    
    //Check for peak
    if(_value > _maxValueRegistered){
        _maxValueRegistered = _value;
        peakButton->setLabel(ofToString(_maxValueRegistered, 2));
    }
}
//------------------------------------------------
void ofxAAMeter::draw(){
    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    
    ofDrawRectangle(_drawRect);
    drawLabel();
    
    if(_enabled){
        drawValueDisplay();
        drawMeter();
    }
    
    ofPopStyle();

    if(_bDrawFullDisplay){
        if(_enabled){
            peakButton->draw();
            smoothSlider->drawSimplified();
        }
        onOffToggle->drawTransparent();
    }
    
}
//------------------------------------------------
void ofxAAMeter::drawMeter(){
    
    ofPushMatrix();
    
    ofTranslate(_x, _y);
    
    ofFill();
    ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA);//change
    
    float scaledValue;
    
    if (_valueNorm >= 0){
        scaledValue = _valueNorm;
    }else{
        scaledValue = ofMap(_value, _minEstimatedValue, _maxEstimatedValue, 0.0, 1.0, true);//clamped value
    }
    
    float meter_h = -1 * (_h * scaledValue);

//    ofDrawRectangle( _w * .25 , _h, _w * 0.5, meter_h);
    ofDrawRectangle( 0 , _h, _w, meter_h);
    
    ofPopMatrix();
    
    
}

//------------------------------------------------
void ofxAAMeter::drawValueDisplay(){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofPushStyle();
    
    //current Value
    ofSetColor(_mainColor);
    string strValue = ofToString(_value, 2);
    
    //align center
    int str_w = verdana->stringWidth(strValue);
    int strVal_x =  _w * 0.5 - str_w * 0.5;

    verdana->drawString(strValue, strVal_x, _line_h * 2.0);
    
    
    ofPopStyle();
    
    ofPopMatrix();
}
//------------------------------------------------
void ofxAAMeter::drawLabel(){
    
    ofPushMatrix();
    
    ofTranslate(_x, _y);
    
    if(_enabled) ofSetColor(_mainColor);
    else ofSetColor(COLOR_ONOFF_OFF);
        
    verdana->drawString(_name, _label_x, _line_h);
    
    //ofDrawBitmapString(_name, _label_x, line_h);
    
    ofPopMatrix();

}

//------------------------------------------------
void ofxAAMeter::resetPeak(){
    _maxValueRegistered = 0.0;
    peakButton->setLabel(ofToString(_maxValueRegistered, 2));

}
//------------------------------------------------
#pragma mark - Setters
//------------------------------------------------
void ofxAAMeter::setPosAndSize(int x, int y, int w, int h){
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _drawRect.set(x, y, w, h);
    
    updateComponentsWidth();
    updateComponentsPositions();
    
}
//------------------------------------------------
void ofxAAMeter::updateComponentsWidth(){
    
    //-:LABEL
    //constraing width
    float label_w = verdana->stringWidth(_name);
    float widthForLabel = _w * 0.95;
    if(label_w >= widthForLabel){
        float space_ratio = 1 / (label_w / widthForLabel);
        verdana->setLetterSpacing(space_ratio);
    }
    //align center
    label_w = verdana->stringWidth(_name);
    _label_x =  _w * .5 - label_w *.5;
    
    
    //-:COMPONENTS
    onOffToggle->setWidth(_w*0.8, 0.0);
    smoothSlider->setWidth(_w*0.8, 0.0);//1.3
    peakButton->setWidth(_w*0.8, 0.0);

}
//------------------------------------------------
//add sizes
void ofxAAMeter::updateComponentsPositions(){
    
    peakButton->setPosition  (_x + _w * 0.1, _y + _line_h * 2.2);
    smoothSlider->setPosition(_x + _w * 0.1, _y + _line_h * 3.2);
    if(_name != MTR_NAME_ONSETS){
        onOffToggle->setPosition (_x + _w * 0.1, _y + _line_h * 4.75);
    }else{
        //onsets On-Off goes lower
        onOffToggle->setPosition (_x + _w * 0.1, _y + _line_h * 6.5);
    }
    
    
}
//------------------------------------------------
void ofxAAMeter::setValue(float val){
    _value = val;
}
//------------------------------------------------
void ofxAAMeter::setNormalizedValue(float val){
    _valueNorm = val;
}
//------------------------------------------------
void ofxAAMeter::setYandHeight(int y, int h){
    setPosAndSize(_x, y, _w, h);
}
//------------------------------------------------
void ofxAAMeter::setHeight(float h){
    _h = h;
    _drawRect.setHeight(h);
}
//------------------------------------------------
void ofxAAMeter::setSmoothAmnt(float val){
    _smoothAmnt = val;
    smoothSlider->setValue(val);
}
//------------------------------------------------
void ofxAAMeter::setEnabled(bool state){
    onOffToggle->setEnabled(state);
    _enabled = state;
}
//------------------------------------------------
void ofxAAMeter::setMainColor(ofColor col){
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
void ofxAAMeter::onSliderEvent(ofxDatGuiSliderEvent e){
    //cout << _name <<"::slider: " <<e.value << endl;
    _smoothAmnt = e.value;
}
//------------------------------------------------
void ofxAAMeter::onButtonEvent(ofxDatGuiButtonEvent e){
    
    if(e.target->getLabel()==MTR_ON_OFF){
        OnOffEventData data;
        data.name = _name;
        data.state = e.enabled;
        data.panelId = _panelId;
        ofNotifyEvent(onOffEventGlobal, data);
        
        _enabled = e.enabled;
     
    }else if(e.target->getType() == ofxDatGuiType::BUTTON){
        resetPeak();
    }

    
}




