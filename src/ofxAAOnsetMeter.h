
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"
#include "ofxAAMetersNames.h"
#include "ofxAudioAnalyzerUnit.h"


class ofxAAOnsetMeter : public ofxAAMeter{
    
public:
    
    ofxAAOnsetMeter(int x, int y, int w, int h, ofxAudioAnalyzerUnit* analyzerPtr);
    
    virtual void update();
    virtual void draw();
    
    virtual void drawMeter();
    
    virtual void setPosAndSize(int x, int y, int w, int h);
    
    void setValue(bool val){_onsetValue = val;}
    void setAlpha(float alpha){
        _alpha = alpha;
        alphaSlider->setValue(alpha);
    }
    void setTreshold(float tres){
        _treshold = tres;
        tresholdSlider->setValue(tres);
    }
    
    bool getValue(){return _onsetValue;}
    float getAlpha(){return _alpha;}
    float getTreshold(){return _treshold;}
    
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
protected:
    
    ofxAudioAnalyzerUnit* audioAnalyzer;
    
    bool _onsetValue;
    float _alpha;
    float _treshold;
    
    ofxDatGuiSlider * alphaSlider;
    ofxDatGuiSlider * tresholdSlider;
};
