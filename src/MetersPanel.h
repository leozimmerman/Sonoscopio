
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

#include"ofxAudioAnalyzer.h"

#include "Panel.h"

#include "ofxAAChannelMetersPanel.h"

class MetersPanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr, ofxAudioAnalyzer* aaPtr);
    void update();
    void draw();
    void exit(){};
    
    void adjustPosAndHeight(int y, int h);
    
    vector<ofxDatGuiComponent*> components;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    
    ofxAAChannelMetersPanel chPanel;
    
    ofxAudioAnalyzer* audioAnalyzerPtr;

    
private:
    
    int guiCompHeight, guiCompWidth;
    
};
