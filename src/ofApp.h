

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



//#define BUFFER_SIZE 512

#define MAIN_PANEL_HEIGHT 0.15
#define TIME_PANEL_HEIGHT 0.4
#define METER_PANEL_HEIGHT 0.45

enum analysisMode{
    MONO,
    SPLIT
};

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
    
    void setAnalysisMode(analysisMode mode);
    void resetAnalysisEngine();
    void setBufferSize(int bs);
    
    void setOscSender(string host, int port);
    void setOscSenderHost(string host);
    void setOscSenderPort(int port);
    void sendOscData();
    void setIsSendingOsc(bool b){_bSendOsc = b;}
    
    string getProjectDir(){return _projectDir;}
    
    void saveSettings();
    void loadSettings();
    
    void openProject(string projectDir);

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
    
    bool _bSendOsc;
    string _oscHost;
    int _oscPort;
    
    analysisMode _currentAnalysisMode;
    string _projectDir;
    
    //---
     ofPolyline waveform;///delete
    
    int  _bufferSize;
    
    
};


