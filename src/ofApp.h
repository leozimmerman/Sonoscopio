

#pragma once

#include "ofMain.h"

#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"

#include "ofxTimeMeasurements.h"
#include "ofxAudioAnalyzer.h"

#include "ofxOsc.h"

#include "MainPanel.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"

#include "AnalysisDataSaver.h"

#define MAIN_PANEL_HEIGHT 0.15
#define TIME_PANEL_HEIGHT 0.4
#define METER_PANEL_HEIGHT 0.45

#define INIT_FPS 30

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
    
    void togglePlay();
    void stop();
    void rewind();
    
    void openAudioFile(string filename);
    void onTimelinePanelResize(int & h);
    
    void setFrameRate(int fps);
    void setAnalysisMode(analysisMode mode);
    void resetAnalysisEngine();
    void setBufferSize(int bs);
    
    void setOscSender(string host, int port);
    void setOscSenderHost(string host);
    void setOscSenderPort(int port);
    void sendOscData();
    void setIsSendingOsc(bool b){_bSendOsc = b;}
    
    int getFrameRate(){return _frameRate;}
    int getSampleRate(){return _samplerate;}
    int getBufferSize(){return _bufferSize;}
    int getChannelsNum(){return _channelsNum;}
    analysisMode getAnalysisMode(){return _currentAnalysisMode;}
    int getTotalFramesNum(){return _totalFramesNum;};
    string getProjectDir(){return _projectDir;}
    
    void saveSettings();
    void loadSettings();
    
    void openProject(string projectDir);
    
    void saveAnalysisDataToFile();
    void drawSavingAnalysisSign();

    //panels-----------
    MainPanel mainPanel;
    TimelinePanel timePanel;
    MetersPanel metersPanel;
    
    ofxAudioAnalyzer mainAnalyzer;
    
    AnalysisDataSaver dataSaver;
    

private:
    
    ofSoundBuffer soundBuffer;
    ofxOscSender oscSender;
    
    int _frameRate;
    int _totalFramesNum;
    
    int _samplerate;
    int  _bufferSize;
    int _channelsNum;
    
    ofMutex audioMutex;
    
    analysisMode _currentAnalysisMode;
    
    bool _bSendOsc;
    string _oscHost;
    int _oscPort;
    bool _bSendOscVectorValues;
    
    string _projectDir;
    
    //---
     ofPolyline waveform;///delete
    
    ofTrueTypeFont	verdana;
    
    
    
    
};


