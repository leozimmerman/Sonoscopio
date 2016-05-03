
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"

class ofxAABinMeter : public ofxAAMeter{

    public:
    
    ofxAABinMeter(string name, int x, int y, int w, int h) : ofxAAMeter(name, x,y,w,h){
        _meterOrient = HORIZONTAL;
    };
    
    //virtual void update();
    virtual void draw();
    
    virtual void drawMeter();
    
    void setBinsNum(int bins){_nBins = bins;}
    void setValues(vector<float>& vals){_values = vals;}
    
    protected:
    vector<float> _values;
    int _nBins;
};