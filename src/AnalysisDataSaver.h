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

#include "TimelinePanel.h"
#include "MetersPanel.h"

#define FILE_INFO_TAG "FILE-INFO"
#define SOUNDFILE_TAG "soundfile"
#define FRAMERATE_TAG "frameRate"
#define FRAMES_NUM_TAG "totalFramesNum"
#define DURATION_TAG "durationSeconds"
#define SAMPLERATE_TAG "sampleRate"
#define BUFFER_SIZE_TAG "bufferSize"
#define CHANNELS_TAG "channelsNum"

#define ANALYSIS_DATA_TAG "ANALYSIS-DATA"
#define FRAME_N_TAG "FRAME-"
#define ANALYZER_TAG "ANALYZER"
#define CHANNEL_N_TAG "CHANNEL-"
#define TIMELINE_TAG "TIMELINE"

class AnalysisDataSaver: public ofThread{

public:
    static AnalysisDataSaver& getInstance(){
        static AnalysisDataSaver instance;
        return instance;
    }

    void reset();

    void start();
    void stop();
    bool drawSavingAnalysisSign();
    
    void threadedFunction();
    
    float getPercentage(){return percentage;};

    void updateFrameRate(int fps, int framesNum);
    
    void setTimelinePanelPtr(TimelinePanel* panelPtr){
        timelinePanelPtr = panelPtr;
    }
    void setMetersPanelPtr(MetersPanel* panelPtr){
        metersPanelPtr = panelPtr;
    }
    
private:
    AnalysisDataSaver();
    AnalysisDataSaver(AnalysisDataSaver const& copy);
    AnalysisDataSaver& operator=(AnalysisDataSaver const& copy);
    
    MetersPanel* metersPanelPtr;
    TimelinePanel* timelinePanelPtr;
    
    string soundfilePath;
    int frameRate;
    int totalFramesNum;
    float durationSecs;
    int sampleRate;
    int bufferSize;
    int channels;
    
    float percentage;
    bool bSaveVectorValues;
    ofTrueTypeFont verdana; //Remove from here.


};
