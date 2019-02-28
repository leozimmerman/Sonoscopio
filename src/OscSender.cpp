//
//  OscSender.cpp
//  Sonoscopio
//
//  Created by Leo on 03/09/2018.
//

#include "OscSender.h"
#include "Macros.h"

OscSender::OscSender(){
    bSendVectorValues = INIT_OSC_SEND_VECTOR;
}

void OscSender::setHost(string host){
    setup(host, getPort());
}

void OscSender::setPort(int port){
    setup(getHost(), port);
}

void OscSender::setSendVectorValues(bool send){
    bSendVectorValues = send;
}

void OscSender::sendOscData(){
    if (metersPanelPtr == NULL || timelinePanelPtr == NULL){
        return;
    }
    
    auto metersValues = metersPanelPtr->getMetersValues();
    auto metersVectorValues = metersPanelPtr->getMetersVectorValues();
    auto trackValues = timelinePanelPtr->getTracksValues();
    sendOscData(metersValues, metersVectorValues, trackValues, bSendVectorValues);
}

void OscSender::sendOscData(const vector<std::map<string, float>> & metersValues, const vector<std::map<string, vector<float>>> & metersVectorValues, const std::map<string, float> & timelineValues, bool sendVectorValues){
    
    bool bWrapInBundle = false;
    
    //-:Send Meters Values
    if(metersVectorValues.size() != metersValues.size()){
        ofLogError()<<"ofApp sendOscData: metersValues and metersVectorValues not matching.";
        return;
    }
    
    for(int i=0; i<metersValues.size(); i++){
        ///'i' -> channel -- For now it will only be 0 as it is only MONO

        //-:Send Single Values
        for (auto const& map : metersValues[i]){
            auto key = map.first;
            auto value = map.second;
            ofxOscMessage msg;
            msg.setAddress("/" + key);
            msg.addFloatArg(value);
            sendMessage(msg, bWrapInBundle);
        }
        
        if(sendVectorValues){
            for (auto const& map : metersVectorValues[i]){
                ofxOscMessage vectorMessage;
                auto key = map.first;
                vectorMessage.setAddress("/" + key);
                for (int j=0; j<metersVectorValues[i].at(key).size(); j++){
                    float value = metersVectorValues[i].at(key)[j];
                    vectorMessage.addFloatArg(value);
                }
                sendMessage(vectorMessage, bWrapInBundle);
            }
        }
    }
    
    //-:Send Timeline Tracks Values (one msg x each track)
    for (auto& kv : timelineValues){
        string key = kv.first;
        float floatValue = kv.second;
        ofxOscMessage msg;
        msg.setAddress("/" + key);//address: "/trackName"
        msg.addFloatArg(floatValue);
        sendMessage(msg, bWrapInBundle);
    }
}
