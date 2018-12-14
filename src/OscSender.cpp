//
//  OscSender.cpp
//  Sonoscopio
//
//  Created by Leo on 03/09/2018.
//

#include "OscSender.h"
#include "Macros.h"

OscSender::OscSender(){
    bSendVectorValues = false;
}

void OscSender::setHost(string host){
    setup(host, getPort());
}

void OscSender::setPort(int port){
    setup(getHost(), port);
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
    
    //-------------------------------------------------
    //-:Send Meters Values

    if(metersVectorValues.size() != metersValues.size()){
        ofLogError()<<"ofApp sendOscData: metersValues and metersVectorValues not matching.";
        return;
    }
    
    for(int i=0; i<metersValues.size(); i++){
        
        //"i" -> channel
        
        //Send Single Values-----------------
        ofxOscMessage msg;
        
        //address: "/ch0" - "/ch1" - "/ch2" etc...
        string address = "/ch" + ofToString(i);
        msg.setAddress(address);
        
        for (auto const& map : metersValues[i]){
           // auto key = map.first;
            auto value = map.second;
            msg.addFloatArg(value);
        }
        
        sendMessage(msg, false);//???ADD OSC BUNDLES?
        
        if(sendVectorValues){
            for (auto const& map : metersVectorValues[i]){
                ofxOscMessage vectorMessage;
                auto key = map.first;
                address = "/ch" + ofToString(i) + key;
                vectorMessage.setAddress(address);
                
                for (int j=0; j<metersVectorValues[i].at(key).size(); j++){
                    float value = metersVectorValues[i].at(key)[j];
                    vectorMessage.addFloatArg(value);
                }
                sendMessage(vectorMessage, false);
            }
        }
        
    }
    
    //-------------------------------------------------
    //-:Send Timeline Tracks Values (one msg x each track)
    //std::map<string, float> timelineValues = timePanel.getTracksValues();
    for (auto& kv : timelineValues){
        string key = kv.first;
        float floatValue = kv.second;
        
        ofxOscMessage msg;
        msg.setAddress("/" + key);//address: "/TL-(trackName)"
        msg.addFloatArg(floatValue);
        sendMessage(msg, false);
        
    }
    
}
