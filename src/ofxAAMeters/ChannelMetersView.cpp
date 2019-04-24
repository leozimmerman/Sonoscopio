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

#include "ChannelMetersView.h"
#include "StringUtils.h"

#pragma mark - Core funcs

ChannelMetersView::ChannelMetersView(int x, int y, int width, int height, int panelId, ofxAudioAnalyzerUnit * audioAnalyzer, vector<ofxAAValue>& enabledValues, vector<ofxAABinsValue>& enabledBinValues, ofColor mainColor){
   
    View::setup(x, y ,width, height);
    
    audioAnalyzerUnit = audioAnalyzer;
    _enabledValueTypes = enabledValues;
    _enabledBinsValueTypes = enabledBinValues;
    
    _mainColor = mainColor;
    _panelId = panelId;
    
    createMeters();
}

void ChannelMetersView::createMeters(){
    meters.clear();
    meters.shrink_to_fit();
    for (auto type : _enabledValueTypes) {
        auto meterView = MeterView::createMeterView(type, _panelId, audioAnalyzerUnit);
        meters.push_back(meterView);
    }
    for (auto type : _enabledBinsValueTypes) {
        auto meterView = BinMeterView::createBinMeterView(type, _panelId, audioAnalyzerUnit);
        meters.push_back(meterView);
    }
    setColors();
    resize(_x, _y, _w, _h);
}

void ChannelMetersView::update(){
    for(int i=0; i<meters.size(); i++){
       meters[i]->update();
    }
}

void ChannelMetersView::draw(){
    View::draw();
    for(int i=0; i<meters.size(); i++){
        meters[i]->draw();
    }
}

void ChannelMetersView::exit(){
    for (auto m : meters) {
        delete m;
    }
    meters.clear();
    meters.shrink_to_fit();
    ofLogVerbose()<<"ofxAAChannelMetersPanel exit.";
}


void ChannelMetersView::resize(int x, int y, int w, int h) {
    View::resize(x, y, w, h);
    int metersWidth = _w;
    
    int y_pos = 0;
    for (auto m : meters) {
        int h = getHeightForMeter(m);
        m->resize(_x, _y + y_pos + _scrollOffset, metersWidth, h);
        y_pos += h;
    }
    _contentHeight = y_pos;
}

void ChannelMetersView::setClicksEnabled(bool enabled){
    View::setClicksEnabled(enabled);
    for (auto m : meters) {
        m->setClicksEnabled(enabled);
    }
}

#pragma mark - Gral funcs

void ChannelMetersView::scrollUp(){
    _scrollOffset += 40;
    if (_scrollOffset > 0) {_scrollOffset = 0;}
    resize(_x , _y, _w, _h);
}

void ChannelMetersView::scrollDown(){
    if (_contentHeight < _h) {return;}
    _scrollOffset -= 40;
    if (_scrollOffset < (_contentHeight-_h) * (-1)) {
        _scrollOffset = (_contentHeight-_h) * (-1);
    }
    resize(_x, _y, _w, _h);
}

void ChannelMetersView::setEnabledAlgorithms(vector<ofxAAValue>& enabledValues, vector<ofxAABinsValue>& enabledBinValues){
    _enabledValueTypes = enabledValues;
    _enabledBinsValueTypes = enabledBinValues;
    createMeters();
}

void ChannelMetersView::setColors(){
    ofColor lightCol = _mainColor;
    lightCol.setBrightness(30);//darker mainColor
    setBackgroundColor(lightCol);
    for (auto m : meters){
        m->setMainColor(_mainColor);
        if(m->getValueType() == ONSETS){
            OnsetMeterView* om = dynamic_cast<OnsetMeterView*>(m);
            om->updateComponentsColors();//update sliders colors
        }
    }
}

int ChannelMetersView::getHeightForMeter(MeterView *meter) {
    if (dynamic_cast<BinMeterView*>(meter)) {
        return BinMeterView::height;
    } else if (dynamic_cast<OnsetMeterView*>(meter))  {
        return OnsetMeterView::height;
    } else {
        return MeterView::height;
    }
}

#pragma mark - Settings

void ChannelMetersView::setStateFromSettings(ChannelMeterSettings& settings){
    for (auto ms : settings.meters) {
        string stringType = ms.type;
        
        ofxAAValue valueType = utils::stringToValueType(stringType);
        
        MeterView* meter = NULL;
        if (valueType != NONE){
            meter = meterOfType(valueType);
        } else {
            ofxAABinsValue binsValueType = utils::stringToBinsValueType(stringType);
            meter = meterOfType(binsValueType);
        }
        
        if (meter != NULL){
            meter->setEnabled(ms.bState);
            if (valueType == ONSETS) {
                auto onsets_m = dynamic_cast<OnsetMeterView*>(meter);
                onsets_m->setAlpha(ms.alpha);
                onsets_m->setSilenceThreshold(ms.silenceTreshold);
                onsets_m->setTimeThreshold(ms.timeTreshold);
            } else {
                meter->setSmoothAmnt(ms.smooth);
                meter->setMaxEstimatedValue(ms.maxEstimatedValue);
                meter->toggleValuePlotter(ms.bPlotValue);
            }
        }
        
    }
}

void ChannelMetersView::loadStateIntoSettings(ChannelMeterSettings* settings){
    settings->meters.clear();
    settings->meters.shrink_to_fit();
    for (auto m : meters) {
        MeterSettings s;

        auto valueType = m->getValueType();
        
        if (valueType != NONE){
            s.type = utils::valueTypeToString(valueType);
        } else {
            auto binMeter = dynamic_cast<BinMeterView*>(m);
            s.type = utils::binsValueTypeToString(binMeter->getBinsValueType());
        }
        
        if (valueType == ONSETS) {
            auto onsets_m = dynamic_cast<OnsetMeterView*>(m);
            s.alpha = onsets_m->getAlpha();
            s.silenceTreshold = onsets_m->getSilenceThreshold();
            s.timeTreshold = onsets_m->getTimeThreshold();
            s.bState = onsets_m->getEnabled();
        } else {
            s.bState = m->getEnabled();
            s.bPlotValue = m->getPlotterEnabled();
            s.smooth = m->getSmoothAmnt();
            s.maxEstimatedValue = m->getMaxEstimatedValue();
        }
        settings->meters.push_back(s);
    }
    
}

MeterView* ChannelMetersView::meterOfType(ofxAAValue type) {
    for (auto m: meters) {
        if (m->getValueType() == type) {
            return m;
        }
    }
    return NULL;
}

MeterView* ChannelMetersView::meterOfType(ofxAABinsValue valueType){
    for (auto m: meters) {
        auto binMeter = dynamic_cast<BinMeterView*>(m);
        if (binMeter != NULL){
            if (binMeter->getBinsValueType() == valueType) {
                return m;
            }
        }
    }
    return NULL;
}

#pragma mark - Value getters

map<string, float> ChannelMetersView::getMetersValues(){
    std::map<string, float> channelMap;
    for(MeterView* m : meters){
        auto type = m->getValueType();
        if (type == ONSETS){
            string key =  m->getName();
            OnsetMeterView* om = dynamic_cast<OnsetMeterView*>(m);
            channelMap[key] = om->getValue();
        }else if(type != NONE){
            string key = m->getName();
            channelMap[key]= m->getNormalizedValue();
        }
    }
    return channelMap;
}

map<string, vector<float>> ChannelMetersView::getMetersVectorValues(){
    std::map<string, vector<float>> channelMap;
    for(MeterView* m : meters){
        auto valueType = m->getValueType();
        if (valueType == NONE){
            BinMeterView* binMeter = dynamic_cast<BinMeterView*>(m);
            string key = m->getName();
            channelMap[key] = binMeter->getValues();
        }
    }
    return channelMap;
}
