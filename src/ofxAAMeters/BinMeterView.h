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

class BinMeterView : public MeterView{

 public:
    
    
    static BinMeterView* createBinMeterView(ofxAABinsValue valueType, int panelId,  ofxAudioAnalyzerUnit * aaPtr);

    BinMeterView(ofxAABinsValue valueType, int panelId,  ofxAudioAnalyzerUnit * aaPtr);
    
    //void draw() override;
    void drawStaticElements() override;
    void drawValueElements() override;

    void updateValues() override;
    void drawMeter() override;
    
    void setBinsNum(unsigned long bins){_nBins = bins;}
    void setValues(vector<float>& vals){_values = vals;}
    void setMinMaxEstimatedValues();
    void setMaxEstimatedValue(float value) override;
    
    void setComponentsPositions() override;
    void setComponentsWidth() override;
    
    vector<float>& getValues(){return _values;}
    static int height;
    
    ofxAABinsValue getBinsValueType(){return _binsValueType;}
    
 protected:
    
    ofxAABinsValue _binsValueType;
    
    vector<float> _values;
    unsigned long _nBins;
};
