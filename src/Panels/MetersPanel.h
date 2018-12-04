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

#include "ofxAudioAnalyzerUnit.h"

#include "Macros.h"
#include "BasePanel.h"
#include "MetersView.h"
#include "MetersPanelGuiView.h"

#define MT_GUI_COMP_HEIGHT 26

#ifndef METERSPANEL_H
#define METERSPANEL_H

class MetersPanelGuiView;

//#ifdef _MSC_VER
//#pragma once
//#endif  // _MSC_VER

class MetersPanel : public BasePanel {
    
public:
    
    void setup(int x, int y, int w, int h) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void exit() override;
    
    virtual bool getFocused() override;
    
    virtual void resize(int x, int y, int w, int h) override;
    virtual void saveSettings(string rootDir="") override;
    virtual void loadSettings(string rootDir="") override;
    
    void setChannelAnalyzers(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms);
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    
    void keyPressed(int key) override;
    
    void scrollUp(){
        metersView.scrollUp();
    }
    void scrollDown(){
        metersView.scrollDown();
    }
    
    vector<std::map<string, float>>& getMetersValues(){
        return metersView.getMetersValues();
    }
    vector<std::map<string, vector<float>>>& getMetersVectorValues(){
        return metersView.getMetersVectorValues();
    }
    
private:
    void setAnalyzerMaxEstimatedValue(ofxAAAlgorithmType algorithm, float value);
    
    MetersView metersView;
    MetersPanelGuiView guiView;
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    vector<ofxAAAlgorithmType> _enabledAlgorithmTypes;
    
    /*
     TODO: Mover a Meters Popup
     ofxDatGuiTextInput* gMaxFreq;
     ofxDatGuiTextInput* gMaxHfc;
     ofxDatGuiTextInput* gMaxCentroid;
     ofxDatGuiTextInput* gMaxSpecComp;
     ofxDatGuiTextInput* gMaxRollOff;
     ofxDatGuiTextInput* gMaxOddEven;
     */

    
};
#endif
