
#pragma once

#include "ofMain.h"
#include "Panel.h"

#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"


class TimelinePanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr);
    void update();
    void draw();
    void exit(){};
    
    void openAudioFile(string filename);
    string getFileInfo();
    
    void startStopPlaying();
        
    ofxTimeline timeline;
    ofxTLAudioTrack* track;
    
private:

    
};

