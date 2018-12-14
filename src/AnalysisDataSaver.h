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

#define RENDER_FILE_INFO_TAG "FILE-INFO"
#define RENDER_SOUNDFILE_TAG "soundfile"
#define RENDER_FRAMERATE_TAG "frameRate"
#define RENDER_FRAMES_NUM_TAG "totalFramesNum"
#define RENDER_DURATION_TAG "durationSeconds"
#define RENDER_SAMPLERATE_TAG "sampleRate"
#define RENDER_BUFFER_SIZE_TAG "bufferSize"
#define RENDER_CHANNELS_NUM_TAG "channelsNum"

#define RENDER_ANALYSIS_DATA_TAG "ANALYSIS-DATA"
#define RENDER_FRAME_N_TAG "FRAME-"
#define RENDER_ANALYZER_TAG "ANALYZER"
#define RENDER_CHANNEL_N_TAG "CHANNEL-"
#define RENDER_TIMELINE_TAG "TIMELINE"


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
    
    string getRenderInfoString();

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
    ofTrueTypeFont verdana; 


};
