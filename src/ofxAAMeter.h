
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxAAMetersNames.h"

enum meterOrientation{
    VERTICAL,
    HORIZONTAL
};

class OnOffEventData{
public:
    string name;
    bool state;
};


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
    
    void resetMaxValue(){_maxValueRegistered = 0.0;}
    
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
    bool getEnabled(){return onOffToggle->getEnabled();}
    bool getIsFullDisplay(){return _bDrawFullDisplay;}

    void setName(string name){_name = name;}
    void setMainColor(ofColor col){_mainColor = col;}
    void setBackgroundColor(ofColor col){_backgroundColor = col;}
    void setValue(float val);
    virtual void setPosAndSize(int x, int y, int w, int h);
    virtual void setYandHeight(int y, int h);
    void setHeight(float h);
    void setMinEstimatedValue(float val){_minEstimatedValue = val;}
    void setMaxEstimatedValue(float val){_maxEstimatedValue = val;}
    void setSmoothAmnt(float val);
    void setEnabled(bool state);
    void setFullDisplay(bool b){_bDrawFullDisplay = b;}

    ofRectangle getDrawRect(){return _drawRect;}
    
    ofxDatGuiSlider * smoothSlider;
    ofxDatGuiButton* onOffToggle;
    
    virtual void onSliderEvent(ofxDatGuiSliderEvent e);
    virtual void onButtonEvent(ofxDatGuiButtonEvent e);
    
    static ofEvent<OnOffEventData> onOffEventGlobal;//this is a shared event for all the instances of this class, so any instance of this class will broadcast to the same event,
    
    
    protected:
    
    float _minEstimatedValue, _maxEstimatedValue;
    
    int _x, _y;
    int _w, _h;
    
    ofRectangle _drawRect;
    

    ofColor _mainColor;
    ofColor _backgroundColor;
    
    ofTrueTypeFont*	verdana;
    float line_h;
    
    meterOrientation _meterOrient;
    bool _bDrawFullDisplay;
   
    
private:
    string _name;
    float _value;
    float _maxValueRegistered;//peak
    float _smoothAmnt;
    int _label_x;
    
    
    
   
    

};