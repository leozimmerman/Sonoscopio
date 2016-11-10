/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * Sonoscopio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofThread.h"

#include "Macros.h"

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

    void updateFrameRate();
    
private:
    
    string soundfilePath;
    int frameRate;
    int totalFramesNum;
    float durationSecs;
    int sampleRate;
    int bufferSize;
    int channelsNum;
    
    
    float percentage;
    
    bool bSaveVectorValues;


};
