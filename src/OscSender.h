//
//  OscSender.hpp
//  Sonoscopio
//
//  Created by Leo on 03/09/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"

#define OSC_FRAME_ADDRESS "/FRAME"

class OscSender: public ofxOscSender {
public:
    
    static OscSender& getInstance(){
        static OscSender instance;
        return instance;
    }
    
    void setHost(string host);
    void setPort(int port);
    void setSendVectorValues(bool send);
    void sendOscData();
    
    void setTimelinePanelPtr(TimelinePanel* panelPtr){
        timelinePanelPtr = panelPtr;
    }
    void setMetersPanelPtr(MetersPanel* panelPtr){
        metersPanelPtr = panelPtr;
    }
    
private:
    OscSender();
    OscSender(OscSender const& copy);
    OscSender& operator=(OscSender const& copy);
    
    
    void sendOscData(int currentFrameNum, const vector<std::map<string, float>> & metersValues, const vector<std::map<string, vector<float>>> & metersVectorValues, const std::map<string, float> & timelineValues, bool sendVectorValues);
    
    bool bSendVectorValues;
    MetersPanel* metersPanelPtr;
    TimelinePanel* timelinePanelPtr;
    
};

