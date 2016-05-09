
#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

#include"ofxAudioAnalyzer.h"

#include "Panel.h"

#include "ofxAAChannelMetersPanel.h"

class MetersPanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr, vector<ofxAudioAnalyzer*>& chanAnalyzerPtrs);
    void update();
    void draw();
    void exit();
    
    
    void reset(vector<ofxAudioAnalyzer*>& chanAnalyzerPtrs);
    void adjustPosAndHeight(int y, int h);
    
    vector<ofxDatGuiComponent*> components;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    
    //ofxAAChannelMetersPanel chPanel;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    vector<ofxAudioAnalyzer*> channelAnalyzers;
    
   

    
private:
    
    int guiCompHeight, guiCompWidth;
    int panelsNum;
    
    
};
