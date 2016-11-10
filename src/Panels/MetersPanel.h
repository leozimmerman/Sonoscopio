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

#include "ofxDatGui.h"

#include"ofxAudioAnalyzerUnit.h"

#include "Macros.h"
#include "Panel.h"
#include "ofxAAChannelMetersPanel.h"

#define MT_GUI_COMP_HEIGHT 26

class MetersPanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr, vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void update();
    void draw();
    void exit();
    
    void saveSettings(string rootDir="");
    void loadSettings(string rootDir="");
    
    void toggleFullDisplay(){setFullDisplay(!_bDrawFullDisplay);}
    void setFullDisplay(bool b);
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    
    void resize(int y, int w, int h);
    void adjustPosAndHeight(int y, int h);
    
    vector<std::map<string, float>>& getMetersValues();
    vector<std::map<string, vector<float>>>& getMetersVectorValues();
    
    bool getIsFullDisplay(){return _bDrawFullDisplay;}
    
    void setupGui();
    void adjustGuiSize(int y, int w, int h);
    
    bool getFocused();

private:
    
    void setAnalyzerMaxEstimatedValue(ofxAAAlgorithm algorithm, float value);
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    ofColor panelColor1, panelColor2;
    
    int panelsNum;
    
    string _panelDir;
    bool _bDrawFullDisplay;
    
    vector < std::map<string, float> > singleValuesForOsc;
    vector < std::map<string, vector<float> > > vectorValuesForOsc;
    
    ofColor bordCol;
    int bordWidth;
    int _guiCompHeight;
    vector<ofxDatGuiComponent*> components;
    ofxDatGuiTextInput* gMaxFreq;
    ofxDatGuiTextInput* gMaxHfc;
    ofxDatGuiTextInput* gMaxCentroid;
    ofxDatGuiTextInput* gMaxSpecComp;
    ofxDatGuiTextInput* gMaxRollOff;
    ofxDatGuiTextInput* gMaxOddEven;
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);

    
    
    
    
};
