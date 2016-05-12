
#pragma once

#include "ofMain.h"

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
    
    void saveSettings(string rootDir="");
    void loadSettings(string rootDir="");
    
    void reset(vector<ofxAudioAnalyzer*>& chanAnalyzerPtrs);
    void adjustPosAndHeight(int y, int h);
    
    std::map<string, float> getMetersValues();
    

private:
    
    vector<ofxAudioAnalyzer*> channelAnalyzers;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    ofColor panelColor1, panelColor2;
    
    int panelsNum;
    
    string _panelDir;
    
    
};
