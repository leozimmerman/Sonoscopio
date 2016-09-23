
#pragma once

#include "ofMain.h"


#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"
#include "ofxDatGui.h"

#include "Directories.h"
#include "Panel.h"

#define PAGE_AUDIO_NAME "PageAudio"
#define PAGE_TRACKS_NAME "PageTracks"

#define TL_GUI_COMP_HEIGHT 26

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
    
    void setFrameRate(int fps);
    
    void loadSettings(string rootDir="");
    void saveSettings(string rootDir="");
    
    void setupGui();
    void adjustGuiSize(int y, int w, int h);
    
    void resize(int y, int w, int h);
    void resizeHeight(int tl_h);
    
    void addMarker();
    void addMarker(float millis);
    void clearMarkers();
    
    void openAudioFile(string filename);
    string getFileInfo();
    
    
    
    bool getIfIsDragging(){return _footerIsDragging;}
    
    void checkIfHeightChanged();
    void checkIfWaveformPreviewChanged();
    
    void addTrack(string name, trackType type);
    void removeTrack(string name);
    void toggleShowTracks();
    void adjustTracksHeight();
    void toggleEnableDisableFocusedTrack();
    void expandFocusedTrack();
    
    std::map<string, float> getTracksValues();

    void bangFired(ofxTLBangEventArgs& args);
    
    ofxTimeline timeline;
    ofxTLAudioTrack* audioTrack;
    
    ofEvent<int> heightResizedEvent;
    
    bool getFocused(){return gTrackName->getFocused();}
    
    
private:
    
    int _frameRate;
    
    int _guiCompHeight;
    string currentTrackName;
    trackType currentTrackType;
    
    bool _isThereBang = false;
    ofxTLTrack* _bangedTrack;
    
    ofColor waveformCol;
    
    ofColor bordCol;
    int bordWidth;
    
    bool _footerIsDragging;
    
    vector<float> _markers;
    
    vector<ofxDatGuiComponent*> components;
    ofxDatGuiTextInput* gTrackName;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
};

