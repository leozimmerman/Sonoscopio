#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofThread.h"

#include "Directories.h"

class AnalysisDataSaver: public ofThread{

public:

    void setup(ofBaseApp* appPtr);
    
    void reset();
    
    void start();
    
    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop();
    
    void threadedFunction();
    
    float getPercentage(){return percentage;};

private:
    
    int frameRate;
    int totalFramesNum;
    int sampleRate;
    int bufferSize;
    int channelsNum;
    
    float percentage;


};