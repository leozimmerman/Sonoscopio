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

#include "MeterView.h"
#include "BinMeterView.h"
#include "OnsetMeterView.h"


#include "ofxAudioAnalyzerUnit.h"
#include "ofxXmlSettings.h"
#include "View.h"
#include "Settings.h"

#define PITCH_MIN_VALUE_FOR_METER 130.0 //hz
#define VERT_METERS_WIDTH 0.8//Width percentage for Vertical Meters

#define VERT_METERS_NUM 12
#define HORIZ_METERS_NUM 5



class ChannelMetersView : public View {
public:

    ChannelMetersView(int x, int y, int width, int height, int panelId, ofxAudioAnalyzerUnit * audioAnalyzer, vector<ofxAAAlgorithmType>& enabledAlgorithms, ofColor mainColor);
    ~ChannelMetersView(){}
    
    virtual void update();
    virtual void draw();
    virtual void exit();
    
    void resize(int x, int y, int w, int h);
    
    void scrollUp();
    void scrollDown();
    
    void setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms);
    
    void setStateFromSettings(ChannelMeterSettings& settings);
    void loadStateIntoSettings(ChannelMeterSettings* settings);
    
    map<string, float> getMetersValues();
    map<string, vector<float>> getMetersVectorValues();
    
protected:
    void createMeters();
    void setColors();
    MeterView* meterOfType(ofxAAAlgorithmType type);
    int getHeightForMeter(MeterView* meter);
    
    vector<MeterView*> meters;
    vector<ofxAAAlgorithmType> enabledAlgorithmTypes;
    ofxAudioAnalyzerUnit* audioAnalyzerUnit;
    
    int _contentHeight;
    ofColor _mainColor;
    int _panelId;
    int _scrollOffset = 0;
    
    

};
