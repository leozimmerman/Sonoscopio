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

#include"ofxAudioAnalyzerUnit.h"

#include "Macros.h"
#include "BasePanel.h"
#include "MetersView.h"
#include "MTGuiView.h"

#define MT_GUI_COMP_HEIGHT 26

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
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    
    void keyPressed(int key) override;
    
    MetersView metersView;//TODO: Make private
    
    
    
private:
    MTGuiView guiView;
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    
    void setAnalyzerMaxEstimatedValue(ofxAAAlgorithmType algorithm, float value);
    
    
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
