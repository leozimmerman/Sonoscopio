
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"


enum meterOrientation{
    VERTICAL,
    HORIZONTAL
};

class ofxAAMeter{
public:
    
    ofxAAMeter(string name, int x, int y, int w, int h);
    //virtual ~ofxAAMeter();

    //void setup();
    virtual void update();
    virtual void draw();

    virtual void drawLabel();
    virtual void drawMeter();
    
    string getName(){return _name;}
    ofColor getMainColor(){return _mainColor;}
    ofColor getBackgroundColor(){return _backgroundColor;}
    float getValue(){return _value;}
    int getPositionX(){return _x;}
    int getPositionY(){return _y;}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    float getMinValue(){return _minValue;}
    float getMaxValue(){return _maxValue;}
    meterOrientation getMeterOrient(){return _meterOrient;}
    float getSmoothAmnt(){return _smoothAmnt;}

    void setName(string name){_name = name;}
    void setMainColor(ofColor col){_mainColor = col;}
    void setBackgroundColor(ofColor col){_backgroundColor = col;}
    void setValue(float val);
    virtual void setPosAndSize(int x, int y, int w, int h);
    virtual void setYandHeight(int y, int h);
    void setHeight(float h);
    void setMinValue(float val){_minValue = val;}
    void setMaxValue(float val){_maxValue = val;}
    
 

    ofRectangle getDrawRect(){return _drawRect;}
    
    ofxDatGuiSlider * smoothSlider;
    ofxDatGuiButton* onOffToggle;
    
    virtual void onSliderEvent(ofxDatGuiSliderEvent e);
    virtual void onButtonEvent(ofxDatGuiButtonEvent e);
    
    ofEvent<bool> stateChangedEvent;
    
    protected:
    
    float _minValue, _maxValue;
    
    int _x, _y;
    int _w, _h;
    
    ofRectangle _drawRect;
    string _name;

    ofColor _mainColor;
    ofColor _backgroundColor;
    
    ofTrueTypeFont	verdana;
    float line_h;
    
    meterOrientation _meterOrient;
    
    
    
private:
    
    float _value;
    float _smoothAmnt;

    
   
    

};