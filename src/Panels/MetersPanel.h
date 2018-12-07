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

#pragma once

#include "ofMain.h"
#include "Macros.h"
#include "BasePanel.h"
#include "MetersView.h"
#include "MetersPanelGui.h"
#include "ofxAudioAnalyzer.h"
#include "Settings.h"

class MetersPanel : public BasePanel {
    
public:
    
    void setup(int x, int y, int w, int h) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void exit() override;
    
    void keyPressed(int key) override;
    
    virtual bool getFocused() override;
    
    virtual void resize(int x, int y, int w, int h) override;
    
    void loadSettings(MetersPanelSettings& settings);
    void updateCurrentSettings();
    MetersPanelSettings* getCurrentSettingsPtr(){
        return &currentSettings;
    }
    
    void setupAnalyzer(int sampleRate, int bufferSize, int channels);
    void analyzeBuffer(const ofSoundBuffer& inBuffer);
    void resetAnalyzer(int sampleRate);
    
    void setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms);
    
    void scrollUp(){metersView.scrollUp();}
    void scrollDown(){metersView.scrollDown();}
    
    int getBufferSize(){return _bufferSize;}
    
    vector<std::map<string, float>>& getMetersValues(){
        return metersView.getMetersValues();
    }
    vector<std::map<string, vector<float>>>& getMetersVectorValues(){
        return metersView.getMetersVectorValues();
    }
    
private:
    
    void setChannelAnalyzers(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void resetAnalyzerUnits(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void setAnalyzerMaxEstimatedValue(ofxAAAlgorithmType algorithm, float value);
    
    ofxAudioAnalyzer audioAnalyzer;
    MetersView metersView;
    MetersPanelGui guiView;
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    vector<ofxAAAlgorithmType> enabledAlgorithmTypes;
    
    MetersPanelSettings currentSettings;
    
    int _bufferSize;
    int _channels;
    
    /*
     TODO: Mover a Meters Modal
     ofxDatGuiTextInput* gMaxFreq;
     ofxDatGuiTextInput* gMaxHfc;
     ofxDatGuiTextInput* gMaxCentroid;
     ofxDatGuiTextInput* gMaxSpecComp;
     ofxDatGuiTextInput* gMaxRollOff;
     ofxDatGuiTextInput* gMaxOddEven;
     */

    
};

