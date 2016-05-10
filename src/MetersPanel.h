
#pragma once

#include "ofMain.h"
//#include "ofxDatGui.h"
#include "ofxXmlSettings.h"
#include"ofxAudioAnalyzer.h"

#include "Panel.h"
#include "ofxAAChannelMetersPanel.h"


#define METERS_SETTINGS_DIR "meters_settings"

class MetersPanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr, vector<ofxAudioAnalyzer*>& chanAnalyzerPtrs);
    void update();
    void draw();
    void exit();
    
    void saveSettings();
    void loadSettings();
    
    void reset(vector<ofxAudioAnalyzer*>& chanAnalyzerPtrs);
    void adjustPosAndHeight(int y, int h);
    
    std::map<string, float> getMetersValues();
    
    //void onButtonEvent(ofxDatGuiButtonEvent e);
    //void onTextInputEvent(ofxDatGuiTextInputEvent e);
    //void onSliderEvent(ofxDatGuiSliderEvent e);
    

private:
    
    //vector<ofxDatGuiComponent*> components;
    //int guiCompHeight, guiCompWidth;
    
    vector<ofxAudioAnalyzer*> channelAnalyzers;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    
    int panelsNum;
    
    ofxXmlSettings XML;
    string _workingDir;
    
    
};
