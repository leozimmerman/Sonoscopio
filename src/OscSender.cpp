//
//  OscSender.cpp
//  Sonoscopio
//
//  Created by Leo on 03/09/2018.
//

#include "OscSender.h"
#include "Macros.h"


void OscSender::setHost(string host){
    setup(host, getPort());
}
//--------------------------------------------------------------
void OscSender::setPort(int port){
    setup(getHost(), port);
}
//--------------------------------------------------------------
void OscSender::sendOscData(const vector<std::map<string, float>> & metersValues, const vector<std::map<string, vector<float>>> & metersVectorValues, const std::map<string, float> & timelineValues, bool sendVectorValues){
    
    //-------------------------------------------------
    //-:Send Meters Values
    
//    vector<std::map<string, float>> metersValues = metersPanel.getMetersValues();
//    vector<std::map<string, vector<float>>> metersVectorValues = metersPanel.getMetersVectorValues();
    
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
        
        //->sames order as Osc Indexes (ofxAudioAnalyzerAlgorithms.h)
        msg.addFloatArg(metersValues[i].at(MTR_NAME_POWER));//0
        msg.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_FREQ));//1
        msg.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_CONF));//2
        msg.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_SALIENCE));//3
        msg.addFloatArg(metersValues[i].at(MTR_NAME_HFC));//4
        msg.addFloatArg(metersValues[i].at(MTR_NAME_CENTROID));//5
        msg.addFloatArg(metersValues[i].at(MTR_NAME_SPEC_COMP));//6
        msg.addFloatArg(metersValues[i].at(MTR_NAME_INHARMONICTY));//7
        msg.addFloatArg(metersValues[i].at(MTR_NAME_DISSONANCE));//8
        msg.addFloatArg(metersValues[i].at(MTR_NAME_ROLL_OFF));//9
        msg.addFloatArg(metersValues[i].at(MTR_NAME_ODD_TO_EVEN));//10
        msg.addInt32Arg(metersValues[i].at(MTR_NAME_ONSETS));//11
        
        sendMessage(msg, false);//???ADD OSC BUNDLES?
        
        if(sendVectorValues){
            
            //Send MelBands-------------------------
            
            ofxOscMessage msgMel;
            
            //address: "/ch0mel" - "/ch1mel" - "/ch2mel" etc...
            address = "/ch" + ofToString(i) + "mel";
            msgMel.setAddress(address);
            
            //cout<<"melbands size: "<< metersVectorValues[i].at(MTR_NAME_MEL_BANDS).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_MEL_BANDS).size(); j++){
                msgMel.addFloatArg(metersVectorValues[i].at(MTR_NAME_MEL_BANDS)[j]);
            }
            sendMessage(msgMel, false);
            
            //Send MFCC-----------------------------
            
            ofxOscMessage msgMfcc;
            
            //address: "/ch0mfcc" - "/ch1mfcc" - "/ch2mfcc" etc...
            address = "/ch" + ofToString(i) + "mfcc";
            msgMfcc.setAddress(address);
            
            //cout<<"mfcc size: "<< metersVectorValues[i].at(MTR_NAME_MFCC).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_MFCC).size(); j++){
                msgMfcc.addFloatArg(metersVectorValues[i].at(MTR_NAME_MFCC)[j]);
            }
            sendMessage(msgMfcc, false);
            
            //Send MFCC-----------------------------
            
            ofxOscMessage msgHpcp;
            
            //address: "/ch0hpcp" - "/ch1hpcp" - "/ch2hpcp" etc...
            address = "/ch" + ofToString(i) + "hpcp";
            msgHpcp.setAddress(address);
            
            //cout<<"hpcp size: "<< metersVectorValues[i].at(MTR_NAME_HPCP).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_HPCP).size(); j++){
                msgHpcp.addFloatArg(metersVectorValues[i].at(MTR_NAME_HPCP)[j]);
            }
            sendMessage(msgHpcp, false);
            
            
            //Send Tristimulus-----------------------------
            
            ofxOscMessage msgTris;
            
            //address: "/ch0tris" - "/ch1tris" - "/ch2tris" etc...
            address = "/ch" + ofToString(i) + "tris";
            msgTris.setAddress(address);
            
            //cout<<"tris size: "<< metersVectorValues[i].at(MTR_NAME_TRISTIMULUS).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_TRISTIMULUS).size(); j++){
                msgTris.addFloatArg(metersVectorValues[i].at(MTR_NAME_TRISTIMULUS)[j]);
            }
            sendMessage(msgTris, false);
            
            
        }
        
    }
    
    //-------------------------------------------------
    //-:Send Timeline Tracks Values (one msg x each track)
    //std::map<string, float> timelineValues = timePanel.getTracksValues();
    for (auto& kv : timelineValues){
        //cout<<"timeline send osc :: "<<kv.first<<" -- "<<kv.second<<endl;
        
        string key = kv.first;
        float floatValue = kv.second;
        
        ofxOscMessage msg;
        msg.setAddress("/" + key);//address: "/TL-(trackName)"
        msg.addFloatArg(floatValue);
        sendMessage(msg, false);
        
    }
    
}
