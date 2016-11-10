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

#include "ofxAAMeter.h"
#include "ofxAudioAnalyzerUnit.h"


class ofxAAOnsetMeter : public ofxAAMeter{
    
public:
    
    ofxAAOnsetMeter(int x, int y, int w, int h, int panelId,  ofxAudioAnalyzerUnit* analyzerPtr);
    
    virtual void update();
    virtual void draw();
    
    virtual void drawMeter();
    
    virtual void setPosAndSize(int x, int y, int w, int h);
    
    
    void updateSlidersColors();
    void updateComponentsWidth();
    void updateComponentsPositions();
    
    void setValue(bool val){_onsetValue = val;}
    void setAlpha(float alpha);
    void setSilenceThreshold(float tres);
    void setTimeThreshold(float tres);
    
    bool getValue(){return _onsetValue;}
    float getAlpha(){return _alpha;}
    float getSilenceThreshold(){return _silenceThreshold;}
    float getTimeThreshold(){return _timeThreshold;}
    
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
protected:
    
    //ofxAudioAnalyzerUnit* audioAnalyzer;
    ofxAAOnsetsAlgorithm* onsets;
    
    bool _onsetValue;
    
    float _alpha;
    float _silenceThreshold;
    float _timeThreshold;
    
    CustomSlider * alphaSlider;
    CustomSlider * silenceThresholdSlider;
    CustomSlider * timeThresholdSlider;
};
