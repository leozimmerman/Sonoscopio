
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ExtendedDatGuiComponents.h"

enum meterOrientation{
    VERTICAL,
    HORIZONTAL
};

class OnOffEventData{
public:
    string name;
    bool state;
};

//-:NAMES
#define MTR_NAME_POWER "POWER"
#define MTR_NAME_PITCH_FREQ "FREQ"
#define MTR_NAME_PITCH_CONF "CONFID"
#define MTR_NAME_PITCH_SALIENCE "SALIENCE"
#define MTR_NAME_HFC "HFC"
#define MTR_NAME_CENTROID "CENTROID"
#define MTR_NAME_SPEC_COMP "SPEC-COMP"
#define MTR_NAME_INHARMONICTY "INHARM"
#define MTR_NAME_DISSONANCE "DISONNANCE"
#define MTR_NAME_ROLL_OFF "ROLL-OFF"
#define MTR_NAME_ODD_TO_EVEN "ODD-EVEN"
#define MTR_NAME_SPECTRUM "SPECTRUM"
#define MTR_NAME_MEL_BANDS "MEL-BANDS"
#define MTR_NAME_MFCC "MFCC"
#define MTR_NAME_HPCP "HPCP"
#define MTR_NAME_TRISTIMULUS "TRISTIMULUS"
#define MTR_NAME_ONSETS "ONSETS"
#define MTR_SMOOTHING "SMTH"
#define MTR_ON_OFF "ON"
#define MTR_PEAK "PEAK"

//-:Meters Colors
#define COLOR_MAIN_A ofColor::darkSeaGreen
#define COLOR_MAIN_B ofColor::darkKhaki


//#define COLOR_MAIN_A ofColor::mediumTurquoise
//#define COLOR_MAIN_B ofColor::lightSalmon

#define COLOR_PEAKS ofColor::crimson
#define COLOR_SMTH_LABEL ofColor::mediumOrchid

//#define COLOR_ONOFF_ON ofColor::chartreuse
#define COLOR_ONOFF_OFF ofColor::dimGray

#define COLOR_RECT_METER ofColor::white
#define COLOR_RECT_METER_ALPHA 40




class ofxAAMeter{
public:
    
    ofxAAMeter(string name, int x, int y, int w, int h);
    virtual ~ofxAAMeter();

    //void setup();
    virtual void update();
    virtual void draw();

    virtual void drawLabel();
    virtual void drawMeter();
    virtual void drawValueDisplay();
    
    void resetPeak();
    
    string getName(){return _name;}
    ofColor getMainColor(){return _mainColor;}
    ofColor getBackgroundColor(){return _backgroundColor;}
    float getValue(){return _value;}
    int getPositionX(){return _x;}
    int getPositionY(){return _y;}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    float getMinEstimatedValue(){return _minEstimatedValue;}
    float getMaxEstimatedValue(){return _maxEstimatedValue;}
    meterOrientation getMeterOrient(){return _meterOrient;}
    float getSmoothAmnt(){return _smoothAmnt;}
    bool getEnabled(){return _enabled;}
    bool getIsFullDisplay(){return _bDrawFullDisplay;}

    void setName(string name){_name = name;}
    void setMainColor(ofColor col);
    void setBackgroundColor(ofColor col){_backgroundColor = col;}
    void setValue(float val);
    void setNormalizedValue(float val);
    virtual void setPosAndSize(int x, int y, int w, int h);
    virtual void updateComponentsPositions();
    virtual void setYandHeight(int y, int h);
    virtual void updateComponentsWidth();
    void setHeight(float h);
    void setMinEstimatedValue(float val){_minEstimatedValue = val;}
    void setMaxEstimatedValue(float val){_maxEstimatedValue = val;}
    void setSmoothAmnt(float val);
    void setEnabled(bool state);
    void setFullDisplay(bool b){_bDrawFullDisplay = b;}

    ofRectangle getDrawRect(){return _drawRect;}
    
    virtual void onSliderEvent(ofxDatGuiSliderEvent e);
    virtual void onButtonEvent(ofxDatGuiButtonEvent e);
    
    static ofEvent<OnOffEventData> onOffEventGlobal;//this is a shared event for all the instances of this class, so any instance of this class will broadcast to the same event,
    
    CustomSlider * smoothSlider;
    OnOffToggle* onOffToggle;
    PeakMeterButton* peakButton;
    
    
protected:
    
    float _minEstimatedValue, _maxEstimatedValue;
    
    int _x, _y;
    int _w, _h;
    
    ofRectangle _drawRect;
    
    ofColor _mainColor;
    ofColor _backgroundColor;
    
    ofTrueTypeFont*	verdana;
    
    meterOrientation _meterOrient;
    bool _bDrawFullDisplay;
    
    string  _name;
    bool    _enabled;
    
    float   _line_h;
    int     _label_x;
    
private:
    
    float _value;
    float _valueNorm;
    float _maxValueRegistered;//peak
    float _smoothAmnt;

    
    
   
    

};