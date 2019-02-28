//
//  Header.h
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 05/12/2018.
//

#pragma once

#include "ofxAAUtils.h"
#include "Macros.h"

struct OscSetting {
    bool bSend = true;
    string host = INIT_OSC_HOST;
    int port = INIT_OSC_PORT;
    bool bSendVectorValues = INIT_OSC_SEND_VECTOR; //TODO: Implement
};

class MainPanelSettings {
public:
    int frameRate = INIT_FPS;
    OscSetting osc;
};

//------------------------------------

struct TLTrackSetting{
    std::string name;
    std::string type;
};

class TimelinePanelSettings {
public:
    float volume = 1.0;
    float bpm = TL_DEFAULT_INIT_BPM;
    bool bLoop = false;
    bool bBpmGrid = false;
    bool bSnap = false;
    bool bFrambased = false;
    
    vector<float> markers;
    vector<TLTrackSetting> tracks;
};


//------------------------------------
class MeterSettings{
public:
    float smooth = 0.0;
    bool bState = true;
    bool bPlotValue = false;
    float alpha;
    float silenceTreshold;
    float timeTreshold;
    float maxEstimatedValue;
    string type;
};

class ChannelMeterSettings{
public:
    vector<MeterSettings> meters;
};

class MetersPanelSettings{
public:
    int bufferSize;
    vector<ofxAAAlgorithmType> enabledAlgorithmTypes;
    vector<ChannelMeterSettings> channelMeters;
};
