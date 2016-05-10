

#pragma once

#include "ofMain.h"

#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"

//#include "ofxAudioAnalyzer.h"
#include "ofxTimeMeasurements.h"
#include "ofxAAMain.h"

#include "ofxOsc.h"

#include "MainPanel.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"



#define BUFFER_SIZE 512

#define MAIN_PANEL_HEIGHT 0.15
#define TIME_PANEL_HEIGHT 0.65
#define METER_PANEL_HEIGHT 0.2

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
    void setOscSenderHost(string host);
    void setOscSenderPort(int port);
    void sendOscData();
    
    void saveSettings();
    void loadSettings();

    
    //panels-----------
    MainPanel mainPanel;
    TimelinePanel timePanel;
    MetersPanel metersPanel;
    
    ofxAAMain mainAnalyzer;
    
    ofSoundBuffer soundBuffer;
    
    ofxOscSender oscSender;
    
   

    
   

private:
    int _channels;
    int _samplerate;
    
    ofMutex audioMutex;
    
    string _oscHost;
    int _oscPort;
    
     ofPolyline waveform;///delete
    
    
    
};


