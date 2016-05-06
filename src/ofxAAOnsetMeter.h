
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"
#include "ofxAAMetersNames.h"
#include "ofxAudioAnalyzer.h"


class ofxAAOnsetMeter : public ofxAAMeter{
    
public:
    
    ofxAAOnsetMeter(int x, int y, int w, int h, ofxAudioAnalyzer* analyzerPtr);
    
    virtual void update();
    virtual void draw();
    
    virtual void drawMeter();
    
    virtual void setPosAndSize(int x, int y, int w, int h);
    
    void setValue(bool val){_onsetValue = val;}
    void setAlpha(float alpha){_alpha = alpha;}
    void setTreshold(float tres){_treshold = tres;}
    
    bool getValue(){return _onsetValue;}
    float getAlpha(){return _alpha;}
    float getTreshold(){return _treshold;}
    
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
protected:
    
    ofxAudioAnalyzer* audioAnalyzer;
    
    bool _onsetValue;
    float _alpha;
    float _treshold;
    
    ofxDatGuiSlider * alphaSlider;
    ofxDatGuiSlider * tresholdSlider;
};
