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
#include "ofApp.h"
#include "GuiFactory.h"
#include "PanelsBridge.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<OnOffEventData> MeterView::onOffEventGlobal = ofEvent<OnOffEventData>();

//FIXME: Move this to somewhere else. Same plase as availableTypes

int MeterView::height = 50;

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

#pragma mark - Inits

MeterView::MeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr) {
    
    _h = MeterView::height;
    _audioAnalyzer = aaPtr;
    _algorithmType = algorithmType;
    _name = ofxaa::algorithmTypeToString(algorithmType);
    _panelId = panelId;
    _mainColor.set(ofColor::cyan);
    setBackgroundColor(ofColor::black);
    _plotterEnabled = false;
    
    initDefaultValues();
    initComponents();
    
    setComponentsWidth();
    setComponentsPositions();
    _framerate = GUI_STATIC_FPS;
    setupMenu();
}

void MeterView::setupMenu(){
    auto ofAppPtr = (ofApp*)ofGetAppPtr();
    menuModal = make_shared<MeterModal>(this);
    menuModal->addListener(ofAppPtr, &ofApp::onModalEvent);
}

void MeterView::showMenu(){
    menuModal->display(ofGetHeight() / 2);
}

void MeterView::initDefaultValues(){
    _value = 0.0;
    _valueNorm = -1.0;
    _minEstimatedValue = 0.0;
    _maxEstimatedValue = _audioAnalyzer->getMaxEstimatedValue(_algorithmType);
    _maxValueRegistered = 0.0;
    _smoothAmnt = 0.0;
    _enabled = TRUE;
    ofColor bordCol = ofColor::grey;
    int bordWidth = 1;
}

void MeterView::initComponents(){
    font  = new ofTrueTypeFont();
    font->load("gui_assets/fonts/verdana.ttf", 10, false, false);
    font->setLineHeight(12.0f);
    font->setLetterSpacing(1.037);
    _line_h = font->getLineHeight();

    onOffToggle = new OnOffToggle(ON_LABEL, TRUE);
    onOffToggle->onButtonEvent(this, &MeterView::onButtonEvent);
    onOffToggle->setHeight(_line_h);
    onOffToggle->setLabelMargin(0.0);
    onOffToggle->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    onOffToggle->setBackgroundColor(ofColor::black);
    
    smoothSlider = new CustomSlider(SMOOTHING_LABEL, 0.0, 1.0, _smoothAmnt);
    smoothSlider->onSliderEvent(this, &MeterView::onSliderEvent);
    smoothSlider->setHeight(_line_h);
    smoothSlider->setLabelMargin(1.0);
    smoothSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    peakButton = new TransparentMeterButton(PEAK_LABEL);
    peakButton->onButtonEvent(this, &MeterView::onButtonEvent);
    peakButton->setHeight(_line_h);
    peakButton->setLabelMargin(0.0);
    peakButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    
    configButton = new TransparentMeterButton(CONFIG_LABEL);
    configButton->setColor(ofColor::white);
    configButton->onButtonEvent(this, &MeterView::onButtonEvent);
    configButton->setLabelMargin(0.0);
    configButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    configButton->setHeight(_line_h);
    
    valuePlotter = GuiFactory::createValuePlotter("", 0.0, 1.0);
    valuePlotter->setHeight(_h);
    valuePlotter->setDrawMode(ofxDatGuiGraph::OUTLINE);
    
}

MeterView::~MeterView(){
    delete smoothSlider;
    delete onOffToggle;
    delete peakButton;
    delete configButton;
    delete valuePlotter;
}

#pragma mark - Update

void MeterView::update(){
    updateValues();
    updateComponents();
    updatePeak();
}

void MeterView::updateComponents(){
    smoothSlider->update();
    onOffToggle->update();
    peakButton->update();
    configButton->update();
    if (_plotterEnabled && PanelsBridge::getInstance().getIsTimelinePlaying ()){
        valuePlotter->setValue(_valueNorm * valuePlotter->getRange());
        valuePlotter->update(true);
    }
    
}

void MeterView::updatePeak(){
    if(_value > _maxValueRegistered){
        _maxValueRegistered = _value;
        peakButton->setLabel(ofToString(_maxValueRegistered, 2));
    }
}

void MeterView::updateValues(){
    setValue(_audioAnalyzer->getValue(_algorithmType, _smoothAmnt));
    setNormalizedValue(_audioAnalyzer->getValue(_algorithmType, _smoothAmnt, TRUE));
}

#pragma mark - Draw

void MeterView::draw(){
    if (!View::mustDrawNewFrame()){
        View::drawLoadedTexture();
        drawValueElements();
        return;
    }
    renderDraw();
}

void MeterView::renderDraw(){
    View::draw();
    ofPushStyle();
    drawValueElements();
    drawStaticElements();
    ofPopStyle();
    View::loadViewInTexture();
}

void MeterView::drawStaticElements(){
    drawLabel();
    onOffToggle->drawTransparent();
    configButton->drawTransparent();
    if(_enabled){
        peakButton->drawTransparent();
        smoothSlider->drawSimplified();
    }
    drawBounds();
}

void MeterView::drawValueElements(){
    if(_enabled){
        if (_plotterEnabled){
            valuePlotter->ofxDatGuiTimeGraph::draw();
        }
        drawValueDisplay();
        drawMeter();
    }
}

void MeterView::drawBounds(){
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_x, _y, _w, _h);
}

void MeterView::drawMeter(){
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofFill();
    ofSetColor(_mainColor, COLOR_RECT_METER_ALPHA);//change
    float scaledValue = (_valueNorm >= 0) ? _valueNorm : ofMap(_value, _minEstimatedValue, _maxEstimatedValue, 0.0, 1.0, true);//clamped value
    ofDrawRectangle(0 , 0, _w * scaledValue, _h);
    ofPopMatrix();
}

void MeterView::drawValueDisplay(){
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofPushStyle();
        ofSetColor(_mainColor);
        string strValue = ofToString(_value, 2);
        //int strVal_x = _x + 5;
        font->drawString(strValue, _label_x, _line_h * 2.0);
    ofPopStyle();
    ofPopMatrix();
}

void MeterView::drawLabel(){
    ofPushMatrix();
    ofTranslate(_x, _y);
        if(_enabled) ofSetColor(_mainColor);
        else ofSetColor(COLOR_ONOFF_OFF);
        font->drawString(_name, _label_x, _line_h);
        //ofDrawBitmapString(_name, _label_x, _line_h);
    ofPopMatrix();
}


void MeterView::resetPeak(){
    _maxValueRegistered = 0.0;
    peakButton->setLabel(ofToString(_maxValueRegistered, 2));
}

#pragma mark - Display set

void MeterView::toggleValuePlotter(bool enabled){
    _plotterEnabled = enabled;
}

void MeterView::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    setComponentsWidth();
    setComponentsPositions();
}

void MeterView::setComponentsWidth(){
    MeterView::adjustFontLetterSpacing( _w * 0.5);
    onOffToggle->setWidth(_w * 0.25, 0.0);
    smoothSlider->setWidth(_w * 0.25, 0.0);
    peakButton->setWidth(_w * 0.25, 0.0);
    configButton->setWidth(_w * 0.25, 0.0);
    valuePlotter->ofxDatGuiTimeGraph::setWidth(_w , 0.0);
}

void MeterView::setComponentsPositions(){
    _label_x = _w * .5 - _label_w *.5;
    onOffToggle->setPosition(_x, _y);
    configButton->setPosition(_x, _y + onOffToggle->getHeight() + 5);
    smoothSlider->setPosition(_x + _w - _w * 0.25, _y);
    peakButton->setPosition  (_x + _w - _w * 0.25, _y + _line_h);
    valuePlotter->ofxDatGuiTimeGraph::setPosition(_x, _y);
}

void MeterView::adjustFontLetterSpacing(int width){
    float widthForLabel = width;
    _label_w = font->stringWidth(_name);
    if(_label_w >= widthForLabel){
        float space_ratio = 1 / (_label_w / widthForLabel);
        font->setLetterSpacing(space_ratio);
    } else {
        font->setLetterSpacing(1.1);
    }
}

#pragma mark - Value Setters

void MeterView::setValue(float value){
    _value = value;
}

void MeterView::setNormalizedValue(float value){
    _valueNorm = value;
}

void MeterView::setMinEstimatedValue(float value){
    _minEstimatedValue = value;
}

void MeterView::setMaxEstimatedValue(float value){
    _maxEstimatedValue = value;
    _audioAnalyzer->setMaxEstimatedValue(_algorithmType, value);
}

void MeterView::setSmoothAmnt(float val){
    _smoothAmnt = val;
    smoothSlider->setValue(val);
}

void MeterView::setEnabled(bool state){
    onOffToggle->setEnabled(state);
    _enabled = state;
}

void MeterView::setMainColor(ofColor col){
    _mainColor = col;
    //colors
    peakButton->setColor(COLOR_PEAKS);
    smoothSlider->setColors(_mainColor, COLOR_SMTH_LABEL, _mainColor);//back,label,fill
    
    //onOffToggle->setColors(COLOR_ONOFF_ON, COLOR_ONOFF_OFF);
    onOffToggle->setColors(_mainColor, COLOR_ONOFF_OFF);
}

#pragma mark - Gui listeners

void MeterView::onSliderEvent(ofxDatGuiSliderEvent e){
    //cout << _name <<"::slider: " <<e.value << endl;
    _smoothAmnt = e.value;
}

void MeterView::onButtonEvent(ofxDatGuiButtonEvent e){
    
    if(e.target->getLabel() == ON_LABEL){
        OnOffEventData data;
        data.type = _algorithmType;
        data.state = e.enabled;
        data.panelId = _panelId;
        ofNotifyEvent(onOffEventGlobal, data);
        
        _enabled = e.enabled;
     
    } else if(e.target->getLabel() == CONFIG_LABEL){
        showMenu();
    } else {
        //peak button's label changes constantly
        resetPeak();
    }

}




