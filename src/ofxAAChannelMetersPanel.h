
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"
#include "ofxAABinMeter.h"
#include "ofxAAOnsetMeter.h"
#include "ofxAAMetersNames.h"

#include "ofxAudioAnalyzerUnit.h"
#include "ofxXmlSettings.h"

#define PITCH_MIN_VALUE_FOR_METER 130.0 //hz
//#define PITCH_MAX_VALUE_FOR_METER 2093.0 //hz
#define VERT_METERS_WIDTH 0.8//Width percentage for Vertical Meters

#define VERT_METERS_NUM 12
#define HORIZ_METERS_NUM 5

class ofxAAChannelMetersPanel{
public:
    
    ofxAAChannelMetersPanel(int x, int y, int width, int height, ofxAudioAnalyzerUnit * aaPtr){
        setup(x, y, width, height, aaPtr);
    }
    ~ofxAAChannelMetersPanel(){}
    virtual void setup(int x, int y, int width, int height, ofxAudioAnalyzerUnit * aaPtr);
    virtual void update();
    virtual void draw();
    virtual void exit();
    
    void setupMeters();
    
    void adjustPosAndHeight(int y, int h);
    
    void setPosition(ofVec2f pos){_x = pos.x; _y = pos.y;}
    void setWidth(int w);
    void setHeight(int h){_h = h;}
    void setBackgroundColor(ofColor col){_bckgColor = col;}
    void setMainColor(ofColor col);
    void setFullDisplay(bool b);
    
    ofVec2f getPosition(){return ofVec2f(_x, _y);}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    ofColor getBackgroundColor(){return _bckgColor;}
    bool getIsFullDisplay(){return _bDrawFullDisplay;}
    
    void onMeterStateChanged(OnOffEventData & data);
    
    vector<ofxAAMeter*> meters;
    
    void loadSettingsFromFile(string filename);
    void saveSettingsToFile(string filename);
    
protected:
    
    ofxAudioAnalyzerUnit* audioAnalyzer;

    int _x, _y;
    int _w, _h;
    ofColor _bckgColor;
    ofColor _mainColor;
    
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
    ofxAAMeter * mDissonance;
    ofxAAMeter * mRollOff;
    ofxAAMeter * mOddToEven;
    
    ofxAAOnsetMeter * mOnsets;
    
    ofxAABinMeter * mSpectrum;
    ofxAABinMeter * mMelBands;
    ofxAABinMeter * mMfcc;//dct
    ofxAABinMeter * mHpcp;
    ofxAABinMeter * mTristimulus;
    
    bool _bDrawFullDisplay;
    


    
    

};