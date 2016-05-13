
#pragma once

#include "ofMain.h"

#include"ofxAudioAnalyzerUnit.h"

#include "Panel.h"
#include "ofxAAChannelMetersPanel.h"


#define METERS_SETTINGS_DIR "meters_settings"

class MetersPanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr, vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void update();
    void draw();
    void exit();
    
    void saveSettings(string rootDir="");
    void loadSettings(string rootDir="");
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void adjustPosAndHeight(int y, int h);
    
    std::map<string, float> getMetersValues();
    

private:
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    ofColor panelColor1, panelColor2;
    
    int panelsNum;
    
    string _panelDir;
    
    
};
