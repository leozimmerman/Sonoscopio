
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
    void setAlpha(float alpha);
    void setSilenceTreshold(float tres);
    void setTimeTreshold(float tres);
    
    bool getValue(){return _onsetValue;}
    float getAlpha(){return _alpha;}
    float getSilenceTreshold(){return _silenceTreshold;}
    float getTimeTreshold(){return _timeTreshold;}
    
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
protected:
    
    ofxAudioAnalyzerUnit* audioAnalyzer;
    
    bool _onsetValue;
    
    float _alpha;
    float _silenceTreshold;
    float _timeTreshold;
    
    ofxDatGuiSlider * alphaSlider;
    ofxDatGuiSlider * silenceTresholdSlider;
    ofxDatGuiSlider * timeTresholdSlider;
};
