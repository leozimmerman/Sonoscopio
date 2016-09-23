
#pragma once

#include "ofMain.h"

#include "ofxDatGui.h"

#include"ofxAudioAnalyzerUnit.h"

#include "Directories.h"
#include "Panel.h"
#include "ofxAAChannelMetersPanel.h"

#define MT_GUI_COMP_HEIGHT 26

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
    vector<std::map<string, vector<float>>>& getMetersVectorValues();
    
    bool getIsFullDisplay(){return _bDrawFullDisplay;}
    
    void setupGui();
    void adjustGuiSize(int y, int w, int h);
    
    bool getFocused();

private:
    
    void setAnalyzerMaxEstimatedValue(ofxAAAlgorithm algorithm, float value);
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;
    vector <ofxAAChannelMetersPanel*> channelPanels;
    
    ofColor panelColor1, panelColor2;
    
    int panelsNum;
    
    string _panelDir;
    bool _bDrawFullDisplay;
    
    vector < std::map<string, float> > singleValuesForOsc;
    vector < std::map<string, vector<float> > > vectorValuesForOsc;
    
    ofColor bordCol;
    int bordWidth;
    int _guiCompHeight;
    vector<ofxDatGuiComponent*> components;
    ofxDatGuiTextInput* gMaxFreq;
    ofxDatGuiTextInput* gMaxHfc;
    ofxDatGuiTextInput* gMaxCentroid;
    ofxDatGuiTextInput* gMaxSpecComp;
    ofxDatGuiTextInput* gMaxRollOff;
    ofxDatGuiTextInput* gMaxOddEven;
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);

    
    
    
    
};
