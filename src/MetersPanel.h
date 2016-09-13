
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
    
    void toggleFullDisplay(){setFullDisplay(!_bDrawFullDisplay);}
    void setFullDisplay(bool b);
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    
    void resize(int y, int w, int h);
    void adjustPosAndHeight(int y, int h);
    
    vector<std::map<string, float>>& getMetersValues();
    bool getIsFullDisplay(){return _bDrawFullDisplay;}
    

private:
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    ofColor panelColor1, panelColor2;
    
    int panelsNum;
    
    string _panelDir;
    bool _bDrawFullDisplay;
    
    vector < std::map<string, float> > valuesForOsc;
    
    
};
