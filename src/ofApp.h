

#pragma once

#include "ofMain.h"

#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"

//#include "ofxAudioAnalyzer.h"
#include "ofxAAMain.h"

#include "ofxOsc.h"

#include "MainPanel.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"

#define BUFFER_SIZE 512

class ofApp : public ofBaseApp{

  public:
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void startStopPlaying();
    
    void openAudioFile(string filename);
    void onTimelinePanelResize(int & h);
    
    void setOscSender(string host, int port);

    
    //panels-----------
    MainPanel mainPanel;
    TimelinePanel timePanel;
    MetersPanel metersPanel;
    
   // ofxAudioAnalyzer audioAnalyzer(44100, 512);
    ofxAAMain mainAnalyzer;
    
    ofSoundBuffer soundBuffer;
    
    ofxOscSender oscSender;

private:
    int _channels;
    int _samplerate;
    
    
    mutex audioMutex;
    
    string _oscHost;
    int _oscPort;
    
    
};


