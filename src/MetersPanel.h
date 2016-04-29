
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

#include "Panel.h"
#include "ofxAAMeter.h"
#include "ofxAABinMeter.h"

class MetersPanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr);
    void update();
    void draw();
    void exit(){};
    
    void adjustPosAndHeight(int y, int h);
    
    vector<ofxDatGuiComponent*> components;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    
    
    ofxAAMeter * meter ;
    ofxAABinMeter * binMeter;
    
    
private:
    
    int guiCompHeight, guiCompWidth;
    
};
