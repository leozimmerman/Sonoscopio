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
#include "ofxAudioAnalyzerUnit.h"

#define MTR_ALPHA "ALPHA"
#define MTR_SIL_TH "SIL-TH"
#define MTR_TI_TH "TI-TH"
#define MTR_ARM "ARM"
#define MTR_RESET "RESET"

class OnsetMeterView : public MeterView{
    
public:
    
    OnsetMeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr);
    virtual ~OnsetMeterView();
    
    void update() override;
    
    void drawStaticElements() override;
    void drawValueElements() override;
    
    void drawMeter() override;
    
    void updateComponentsColors();
    void setComponentsHeight();
    void setComponentsWidth() override;
    void setComponentsPositions() override;
    
    void setValue(bool val);
    void setAlpha(float alpha);
    void setSilenceThreshold(float tres);
    void setTimeThreshold(float tres);
    void setArmed(bool val) {_isArmed = val;}

    bool getValue(){return _onsetValue;}
    float getAlpha(){return _alpha;}
    float getSilenceThreshold(){return _silenceThreshold;}
    float getTimeThreshold(){return _timeThreshold;}
    
    void onSliderEvent(ofxDatGuiSliderEvent e) override;
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    
    /*TODO: Ver onOffEventGlobal si hace falta mandar mas data*/
    static ofEvent<int> onsetEventGlobal;
    static int height;
    
protected:
    void sendOnsetEvent();
    ofxAAOnsetsAlgorithm* onsets;
    
    bool _onsetValue;
    bool _isArmed; //for recording
    
    float _alpha;
    float _silenceThreshold;
    float _timeThreshold;
    
    CustomSlider * alphaSlider;
    CustomSlider * silenceThresholdSlider;
    CustomSlider * timeThresholdSlider;
    OnOffToggle* armToggle;
    TransparentMeterButton * resetButton;
    
};


