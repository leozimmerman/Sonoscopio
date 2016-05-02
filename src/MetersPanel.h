
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

#include "Panel.h"

//#include "ofxAAMeter.h"
//#include "ofxAABinMeter.h"
//#include "ofxAAOnsetMeter.h"

#include "ofxAAMetersPanel.h"

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
    
    
    ofxAAMetersPanel mPanel;
//    ofxAAMeter * meter ;
//    ofxAABinMeter * binMeter;
//    ofxAAOnsetMeter * onsetMeter;
    
private:
    
    int guiCompHeight, guiCompWidth;
    
};
