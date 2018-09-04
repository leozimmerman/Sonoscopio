//
//  Configurations.h
//  Sonoscopio
//
//  Created by Leo on 03/09/2018.
//

#pragma once
enum AnalysisMode{
    MONO,
    SPLIT
};

enum ViewMode{
    ALL,
    TIMEPANEL_ONLY,
    METERSPANEL_ONLY
};

struct OscConfiguration {
    bool bSend;
    string host;
    int port;
    bool bSendVectorValues;
};


class MainConfiguration {
public:
    int getFrameRate(){return frameRate;}
    int getSampleRate(){return sampleRate;}
    int getBufferSize(){return bufferSize;}
    int getChannelsNum(){return channelsNum;}
    string getProjectDir(){return projectDir;}
    AnalysisMode getAnalysisMode(){return analysisMode;}
    ViewMode getViewMode(){return viewMode;}
    OscConfiguration osc(){return oscConf;}
    
    
    void setProjectDir(string s){projectDir = s;}
    void setFrameRate(int i){frameRate = i;}
    void setSampleRate(int i){sampleRate = i;}
    void setBufferSize(int i){bufferSize = i;}
    void setChannelsNum(int i){channelsNum = i;}
    void setAnalysisMode(AnalysisMode m){analysisMode = m;}
    void setViewMode(ViewMode m){viewMode = m;}
    
    
    void setOscConfiguration(string host, int port, bool bSend, bool bSendVectorValues){
        oscConf.host = host;
        oscConf.port = port;
        oscConf.bSend = bSend;
        oscConf.bSendVectorValues = bSendVectorValues;
    }
    void setIsSendingOsc(bool b){oscConf.bSend = b;}
    
    
    
private:
    std::string projectDir;
    int frameRate;
    int sampleRate;
    int bufferSize;
    int channelsNum;
    
    AnalysisMode analysisMode;
    ViewMode viewMode;
    
    OscConfiguration oscConf;
} ;


struct MeterPanelConfiguration{};

