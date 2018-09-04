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



class OscSender: public ofxOscSender {
public:
    
    void setHost(string host);
    void setPort(int port);
    void sendOscData(const vector<std::map<string, float>> & metersValues, const vector<std::map<string, vector<float>>> & metersVectorValues, const std::map<string, float> & timelineValues, bool sendVectorValues);
    
};

