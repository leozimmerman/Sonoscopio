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
#include "ofxAAUtils.h"
#include "SettingsManager.h"

#pragma mark - Core Funcs

void MetersPanel::setup(int x, int y, int w, int h){
    BasePanel::setup(x, y, w, h);
    
    guiView.setup(x, y, w, GUI_COMP_HEIGHT, this);
    metersView.setup(x, guiView.maxY(), w, h - guiView.getHeight());
    
    enabledAlgorithmTypes = ofxaa::allAvailableAlgorithmTypes;
    
    SettingsManager::getInstance().setMetersPanelPtr(this);
}

void MetersPanel::setupAnalyzer(int sampleRate, int bufferSize, int channels){
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    setChannelAnalyzers(audioAnalyzer.getChannelAnalyzersPtrs());
}

void MetersPanel::resetAnalyzer(int sampleRate, int bufferSize, int channels){
    audioAnalyzer.reset(sampleRate, bufferSize, channels);
    resetAnalyzerUnits(audioAnalyzer.getChannelAnalyzersPtrs());
}

void MetersPanel::setChannelAnalyzers(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    channelAnalyzers = chanAnalyzerPtrs;
    metersView.setupChannelMeters(chanAnalyzerPtrs, enabledAlgorithmTypes);
}

void MetersPanel::setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms) {
    enabledAlgorithmTypes = enabledAlgorithms;
    
    for (int ch=0; ch<audioAnalyzer.getChannelsNum(); ch++){
        for (auto algorithm : ofxaa::allAvailableAlgorithmTypes){
            audioAnalyzer.setActive(ch, algorithm, false);
        }
        for (auto algorithm : enabledAlgorithms){
            audioAnalyzer.setActive(ch, algorithm, true);
        }
    }
    metersView.setEnabledAlgorithms(enabledAlgorithms);
}



void MetersPanel::analyzeBuffer(const ofSoundBuffer& inBuffer){
    audioAnalyzer.analyze(inBuffer);
}

void MetersPanel::update(){
    metersView.update();
    guiView.update();
}

void MetersPanel::draw(){
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
    
    View::loadViewInTexture();
}

void MetersPanel::exit(){
    audioAnalyzer.exit();
    metersView.exit();
}

void MetersPanel::keyPressed(int key){
    switch (key) {
        case '1':
            metersView.scrollUp();
            break;
        case '2':
            metersView.scrollDown();
            break;
    }
}

void MetersPanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, GUI_COMP_HEIGHT);
    metersView.resize(x, guiView.maxY(), w, h - guiView.getHeight());
}

#pragma mark - Settings

void MetersPanel::loadSettings(){
    
    metersView.loadSettings();
}
//----------------------------------------------
void MetersPanel::updateCurrentSettings(){
    //TODO: Implement
    metersView.updateCurrentSettings();
}
//----------------------------------------------
#pragma mark - Other funcs
//----------------------------------------------
void MetersPanel::resetAnalyzerUnits(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    metersView.reset(chanAnalyzerPtrs);
}
//----------------------------------------------
bool MetersPanel::getFocused(){
    /**
    if(gMaxFreq->getFocused() ||
       gMaxHfc->getFocused() ||
       gMaxCentroid->getFocused()  ||
       gMaxSpecComp->getFocused()  ||
       gMaxRollOff->getFocused()  ||
       gMaxOddEven->getFocused()   )
    {
        return true;
    }else{
        return false;
    }
    */
    return false;
}

//--------------------------------------------------------------
void MetersPanel::setAnalyzerMaxEstimatedValue(ofxAAAlgorithmType algorithm, float value){
    for (ofxAudioAnalyzerUnit* anUnit : channelAnalyzers){
        anUnit->setMaxEstimatedValue(algorithm, value);
    }
}


