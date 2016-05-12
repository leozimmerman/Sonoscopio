
#pragma once

#include "ofMain.h"
#include "Panel.h"

#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"

#include "ofxDatGui.h"

#define PAGE_AUDIO_NAME "PageAudio"
#define PAGE_TRACKS_NAME "PageTracks"

#define TIMELINE_SETTINGS_DIR "timeline_settings"

enum trackType {
    CURVES,
    BANGS,
    SWITCHES
};

class TimelinePanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr);
    void update();
    void draw();
    void exit(){};
    
    void keyPressed(int key);
    
    void loadSettings(string rootDir="");
    void saveSettings(string rootDir="");
    
    void setupGUI();
    
    void resizeHeight(int tl_h);
    
    void openAudioFile(string filename);
    string getFileInfo();
    
    void startStopPlaying();
    
    void addTrack(string name, trackType type);
    void removeTrack(string name);
    void toggleShowTracks();
    void adjustTracksHeight();
    void toggleEnableDisableFocusedTrack();
    
    std::map<string, float> getTracksValues();

    void bangFired(ofxTLBangEventArgs& args);
    
    vector<ofxDatGuiComponent*> components;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    ofxTimeline timeline;
    ofxTLAudioTrack* audioTrack;
    
    ofEvent<int> heightResizedEvent;
    
    
private:
    
    int guiCompWidth, guiCompHeight;
    string currentTrackName;
    trackType currentTrackType;
    
    bool _isThereBang = false;
    ofxTLTrack* _bangedTrack;
    
    ofColor bordCol;
    int bordWidth;
    
    ofColor waveformCol;
    
};

