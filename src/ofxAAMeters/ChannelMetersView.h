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
    
    void createMeters();
    
    void scrollUp();
    void scrollDown();
    
    void setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms);
    
    ofVec2f getPosition(){return ofVec2f(_x, _y);}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    
    void onMeterStateChanged(OnOffEventData & data);
    
    vector<MeterView*> meters;
    
    void loadSettingsFromFile(string filename);
    void saveSettingsToFile(string filename);
    
    //for osc and data saving
    MeterView* meterForType(ofxAAAlgorithmType type) {
        for (auto m: meters) {
            if (m->getType() == type) {
                return m;
            }
        }
        return nil;
    }
    
    //TODO: Remove this eventually....
    vector<float>& getMelBandsValues(){
        BinMeterView* binMeter = dynamic_cast<BinMeterView*>(meterForType(MEL_BANDS));
        return binMeter->getValues();
    }
    vector<float>& getMfccValues(){
        BinMeterView* binMeter = dynamic_cast<BinMeterView*>(meterForType(MFCC));
        return binMeter->getValues();
    }
    vector<float>& getHpcpValues(){
        BinMeterView* binMeter = dynamic_cast<BinMeterView*>(meterForType(HPCP));
        return binMeter->getValues();
    }
    vector<float>& getTristimulusValues(){
        BinMeterView* binMeter = dynamic_cast<BinMeterView*>(meterForType(TRISTIMULUS));
        return binMeter->getValues();
    }
    int getHeightForMeter(MeterView* meter);
    int getContentHeight(){return _contentHeight;}
    
    
protected:
    void setColors();
    
    vector<ofxAAAlgorithmType> _enabledAlgorithmTypes;
    ofxAudioAnalyzerUnit* _audioAnalyzer;
    int _contentHeight;
    ofColor _mainColor;
    int _panelId;
    int _scrollOffset = 0;

};
