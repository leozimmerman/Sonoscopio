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
#include "ofxAAChannelMetersPanel.h"

#define MT_GUI_COMP_HEIGHT 26

class MetersPanel : public BasePanel {
    
public:
    
    void setup(int x, int y, int width, int height);
    virtual void update() override;
    virtual void draw() override;
    virtual void exit() override;
    
    virtual bool getFocused() override;
    void setupGui();
    virtual void resize(int x, int y, int w, int h) override;
    virtual void saveSettings(string rootDir="") override;
    virtual void loadSettings(string rootDir="") override;
    
    void setChannelAnalyzers(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void toggleFullDisplay(){setFullDisplay(!_bDrawFullDisplay);}
    void setFullDisplay(bool b);
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void adjustPosAndHeight(int y, int h);
    
    vector<std::map<string, float>>& getMetersValues();
    vector<std::map<string, vector<float>>>& getMetersVectorValues();
    
    bool getIsFullDisplay(){return _bDrawFullDisplay;}
    void adjustGuiSize(int y, int w, int h);
    


private:
    
    void setAnalyzerMaxEstimatedValue(ofxAAAlgorithm algorithm, float value);
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    ofColor panelColor1, panelColor2;
    
    int panelsNum;
    bool _bDrawFullDisplay;
    
    vector < std::map<string, float> > singleValuesForOsc;
    vector < std::map<string, vector<float> > > vectorValuesForOsc;
    
   
    ofxDatGuiTextInput* gMaxFreq;
    ofxDatGuiTextInput* gMaxHfc;
    ofxDatGuiTextInput* gMaxCentroid;
    ofxDatGuiTextInput* gMaxSpecComp;
    ofxDatGuiTextInput* gMaxRollOff;
    ofxDatGuiTextInput* gMaxOddEven;
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);

    
    
};
