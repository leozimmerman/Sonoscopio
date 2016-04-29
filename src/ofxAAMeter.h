
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

class ofxAAMeter{
    public:
    
    ofxAAMeter(int x, int y, int w, int h);
    //virtual ~ofxAAMeter();

    void setup();
    virtual void update();
    virtual void draw();

    string getName(){return _name;}
    ofColor getMainColor(){return _mainColor;}
    ofColor getBackgroundColor(){return _backgroundColor;}
    float getValue(){return _value;}

    void setName(string name){_name = name;}
    void setMainColor(ofColor col){_mainColor = col;}
    void setBackgroundColor(ofColor col){_backgroundColor = col;}
    void setValue(float val){_value = val;}
    void setPosAndSize(int x, int y, int w, int h);
    void setHeight(float h);

    ofRectangle getDrawRect(){return _drawRect;}
    
    ofxDatGuiSlider * smoothSlider;
    ofxDatGuiButton* onOffToggle;
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    protected:
    
    int _x, _y;
    int _w, _h;
    
    ofRectangle _drawRect;
    string _name;

    ofColor _mainColor;
    ofColor _backgroundColor;
    
    
     float _value;
    ofTrueTypeFont	verdana;
    
    private:
    
   
    

};