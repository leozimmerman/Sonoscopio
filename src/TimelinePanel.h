
#pragma once

#include "ofMain.h"
#include "Panel.h"

#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"

#include "ofxDatGui.h"


enum trackType {
    CURVES,
    BANGS,
    FLAGS,
    SWITCHES
};

class TimelinePanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr);
    void update();
    void draw();
    void exit(){};
    
    void openAudioFile(string filename);
    string getFileInfo();
    
    void startStopPlaying();
    
    void addTrack(string name, trackType type);
    void removeTrack(string name);
    void collapseAllTracks();
    void expandFocusedTrack();
    void toggleEnableDisableFocusedTrack();
        
    
    
    
    vector<ofxDatGuiComponent*> components;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    ofxTimeline timeline;
    ofxTLAudioTrack* audioTrack;
    ofxTLPage* page;
    
private:
    
    int guiCompWidth, guiCompHeight;
    string currentTrackName;
    trackType currentTrackType;
    
    
    
};

