//
//  MetersView.hpp
//  Sonoscopio
//
//  Created by Leo on 07/09/2018.
//

#pragma once

#include "ofMain.h"
#include "ChannelMetersView.h"

class MetersView : public View {
public:
    void setup(int x, int y, int w, int h) override;
    void setupChannelMeters(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs, vector<ofxAAAlgorithmType> enabledAlgorithms);
    
    void draw() override;
    void update();
    void exit();
    
    void resize(int x, int y, int w, int h) override;
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms);
    void scrollUp();
    void scrollDown();
    
    vector<std::map<string, float>>& getMetersValues();
    vector<std::map<string, vector<float>>>& getMetersVectorValues();
    
private:
    void createChannelMetersViews();
   
    vector <shared_ptr<ChannelMetersView>> _channelMetersViews;
    vector<ofxAAAlgorithmType> _enabledAlgorithmTypes;
    vector<ofxAudioAnalyzerUnit*> _channelAnalyzers;

    vector < std::map<string, float> > singleValuesForOsc;
    vector < std::map<string, vector<float> > > vectorValuesForOsc;
};
