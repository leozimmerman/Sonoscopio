
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
