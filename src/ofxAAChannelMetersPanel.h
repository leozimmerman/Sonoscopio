
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"
#include "ofxAABinMeter.h"
#include "ofxAAOnsetMeter.h"
#include "ofxAAMetersNames.h"

#include"ofxAudioAnalyzer.h"



class ofxAAChannelMetersPanel{
public:
    
    virtual void setup(int x, int y, int width, int height, ofxAudioAnalyzer * aaPtr);
    virtual void update();
    virtual void draw();
    virtual void exit(){};
    
    void setupMeters();
    
    void adjustPosAndHeight(int y, int h);
    
    void setPosition(ofVec2f pos){_x = pos.x; _y = pos.y;}
    void setWidth(int w){_w = w ;}
    void setHeight(int h){_h = h;}
    void setBackgroundColor(ofColor col){_bckgColor = col;}
    
    ofVec2f getPosition(){return ofVec2f(_x, _y);}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    ofColor getBackgroundColor(){return _bckgColor;}
    
    void onMeterStateChanged(OnOffEventData & data);
    
    
protected:
    
    ofxAudioAnalyzer* audioAnalyzer;///**

    int _x, _y;
    int _w, _h;
    ofColor _bckgColor;
    
    int VMetersNum, HMetersNum ;
    int HMetersWidthTotal, VMetersWidthTotal;
    int HMeterWidth, HMeterHeight;
    int VMeterWidth, VMeterHeight;
    
    ofxAAMeter * mPower;
    ofxAAMeter * mPitchFreq;
    ofxAAMeter * mPitchConf;
    ofxAAMeter * mSalience;
    ofxAAMeter * mHfc;
    ofxAAMeter * mCentroid;
    ofxAAMeter * mSpecComp;
    ofxAAMeter * mInharm;
    
    ofxAAOnsetMeter * mOnsets;
    
    ofxAABinMeter * mSpectrum;
    ofxAABinMeter * mMelBands;
    ofxAABinMeter * mMfcc;//dct
    ofxAABinMeter * mHpcp;
    
    vector<ofxAAMeter*> meters;
    
    
    

    
    

};