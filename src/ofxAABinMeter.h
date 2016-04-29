
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"

class ofxAABinMeter : public ofxAAMeter{

    public:
    
    ofxAABinMeter(int x, int y, int w, int h) : ofxAAMeter(x,y,w,h){};
    
    virtual void update();
    virtual void draw();
    
    void setNumBins(int bins){_nBins = bins;}
    void setValues(vector<float>& vals){_values = vals;}
    
    protected:
    
    vector<float> _values;
    int _nBins;
};