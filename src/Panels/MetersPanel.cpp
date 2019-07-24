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

#include "MetersPanel.h"
#include "ofApp.h"
#include "StringUtils.h"
#include "SettingsManager.h"
#include "FileManager.h"
#include "AnalysisDataSaver.h"
#include "OscSender.h"
#include "PanelsBridge.h"

#pragma mark - Core Funcs

void MetersPanel::setup(int x, int y, int w, int h){
    BasePanel::setup(x, y, w, h);
    BasePanel::setEnabled(false);
    setupSingletons();
    
    //enabledAlgorithmTypes = ofxaa::allAvailableAlgorithmTypes;
    
    guiView.setup(x, y, w, GUI_COMP_HEIGHT, this);
    metersView.setup(x, guiView.maxY(), w, h - guiView.getHeight());
    
    setupAnalyzer(INIT_SAMPLE_RATE, INIT_BUFFER_SIZE, 1);
}

void MetersPanel::setupSingletons(){
    SettingsManager::getInstance().setMetersPanelPtr(this);
    FileManager::getInstance().setMetersPanelPtr(this);
    AnalysisDataSaver::getInstance().setMetersPanelPtr(this);
    OscSender::getInstance().setMetersPanelPtr(this);
    PanelsBridge::getInstance().setMetersPanelPtr(this);
}

void MetersPanel::update(){
    if(!enabled){return;}
    
    metersView.update();
    guiView.update();
}

void MetersPanel::draw(){
    if(!enabled){return;}
    
    if (!View::mustDrawNewFrame()){
        View::drawLoadedTexture();
        return;
    }
    
    if (_isHidden){ return; }
    View::draw();
    TS_START("meters");
    metersView.draw();
    TS_STOP("meters");
    
    TS_START("gui-meters");
    guiView.draw();
    TS_STOP("gui-meters");
    
    TS_START("loadViewInTexture");
    View::loadViewInTexture();
    TS_STOP("loadViewInTexture");
}

void MetersPanel::exit(){
    audioAnalyzer.exit();
    metersView.exit();
}

bool MetersPanel::keyPressed(int key){
    switch (key) {
        case OF_KEY_UP:
            scrollUp();
            return true;
            break;
        case OF_KEY_DOWN:
            scrollDown();
            return true;
            break;
    }
    return false;
}

bool MetersPanel::getFocused(){
    return guiView.getFocused();
}

void MetersPanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, GUI_COMP_HEIGHT);
    metersView.resize(x, guiView.maxY(), w, h - guiView.getHeight());
}

void MetersPanel::setClicksEnabled(bool enabled) {
    View::setClicksEnabled(enabled);
    guiView.setClicksEnabled(enabled);
    metersView.setClicksEnabled(enabled);
}

#pragma mark - Audio Analyzer

void MetersPanel::setupAnalyzer(int sampleRate, int bufferSize, int channels){
    _bufferSize = bufferSize;
    _channels = channels;
    currentSettings.bufferSize = bufferSize;
    audioAnalyzer.setup(sampleRate, _bufferSize, _channels);
    setChannelAnalyzers(audioAnalyzer.getChannelAnalyzersPtrs());
}

void MetersPanel::resetAnalyzer(int sampleRate){
    audioAnalyzer.reset(sampleRate, _bufferSize, _channels);
    resetAnalyzerUnits(audioAnalyzer.getChannelAnalyzersPtrs());
}

void MetersPanel::setBufferSize(int bs){
    if (bs == _bufferSize){return;}
    _bufferSize = bs;
    currentSettings.bufferSize = _bufferSize;
    audioAnalyzer.reset(audioAnalyzer.getSampleRate(), _bufferSize, _channels);
    resetAnalyzerUnits(audioAnalyzer.getChannelAnalyzersPtrs());
}

void MetersPanel::setChannelAnalyzers(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    channelAnalyzers = chanAnalyzerPtrs;
    metersView.setupChannelMeters(chanAnalyzerPtrs, _enabledValueTypes, _enabledBinsValueTypes);
}

void MetersPanel::analyzeBuffer(const ofSoundBuffer& inBuffer){
    audioAnalyzer.analyze(inBuffer);
}

#pragma mark - Settings

void MetersPanel::resetSettings(){
    currentSettings = MetersPanelSettings();
    currentSettings.bufferSize = _bufferSize;
    loadSettings(currentSettings);
}

void MetersPanel::loadSettings(MetersPanelSettings& settings){
    currentSettings = settings;
    
    setBufferSize(settings.bufferSize);
    
    _enabledValueTypes = settings.enabledValueTypes;
    _enabledBinsValueTypes = settings.enabledBinsValueTypes;
    metersView.setEnabledAlgorithms(_enabledValueTypes, _enabledBinsValueTypes);//resetsMeters
    
    guiView.setStateFromSettings(settings);
    metersView.setStateFromSettings(settings);
}

void MetersPanel::updateMetersViewSettings(){
    metersView.loadStateIntoSettings(&currentSettings);
}

void MetersPanel::updateCurrentSettings(){
    currentSettings.bufferSize = _bufferSize;
    currentSettings.enabledValueTypes = _enabledValueTypes;
    currentSettings.enabledBinsValueTypes = _enabledBinsValueTypes;
    guiView.loadStateIntoSettings(&currentSettings);
    metersView.loadStateIntoSettings(&currentSettings);
}

#pragma mark - Other funcs

void MetersPanel::setEnabledAlgorithms(vector<ofxAAValue>& enabledValues, vector<ofxAABinsValue>& enabledBinValues) {
    currentSettings.enabledValueTypes = enabledValues;
    currentSettings.enabledBinsValueTypes = enabledBinValues;
    loadSettings(currentSettings);
}

void MetersPanel::resetAnalyzerUnits(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    metersView.reset(chanAnalyzerPtrs);
    metersView.setStateFromSettings(currentSettings);
}


