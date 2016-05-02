
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"

class ofxAAOnsetMeter : public ofxAAMeter{
    
public:
    
    //ofxAAOnsetMeter(int x, int y, int w, int h) : ofxAAMeter(x,y,w,h);
    ofxAAOnsetMeter(int x, int y, int w, int h);
    
    virtual void update();
    virtual void draw();
    
    void setValue(bool val){_onsetValue = val;}
    void setAlpha(float alpha){_alpha = alpha;}
    void setTreshold(float tres){_treshold = tres;}
    
    bool getValue(){return _onsetValue;}
    float getAlpha(){return _alpha;}
    float getTreshold(){return _treshold;}
    
protected:
    
    bool _onsetValue;
    float _alpha;
    float _treshold;
    
    ofxDatGuiSlider * alphaSlider;
    ofxDatGuiSlider * tresholdSlider;
};
