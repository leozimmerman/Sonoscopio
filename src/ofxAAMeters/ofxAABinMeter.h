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


class ofxAABinMeter : public ofxAAMeter{

 public:

    ofxAABinMeter(string name, int x, int y, int w, int h, int panelId);
    
    void draw();
    
    void drawMeter();
    void drawLabel();
    
    void setBinsNum(int bins){_nBins = bins;}
    void setValues(vector<float>& vals){_values = vals;}
    
    //void updateColors();
    void updateComponentsPositions();
    void updateComponentsWidth();
    
    void setPosAndSize(int x, int y, int w, int h);
    
    vector<float>& getValues(){return _values;}
    
 protected:
    
    vector<float> _values;
    int _nBins;
};
