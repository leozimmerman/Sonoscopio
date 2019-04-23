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
    void setupChannelMeters(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs, vector<ofxAAValue>& enabledValues, vector<ofxAABinsValue>& enabledBinValues);
    
    void draw() override;
    void update();
    void exit();
    
    void resize(int x, int y, int w, int h) override;
    void setClicksEnabled(bool enabled) override;
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    void setEnabledAlgorithms(vector<ofxAAValue>& enabledValues, vector<ofxAABinsValue>& enabledBinValues);
    
    void loadStateIntoSettings(MetersPanelSettings* settings);
    void setStateFromSettings(MetersPanelSettings& settings);
    
    void scrollUp();
    void scrollDown();
    
    vector<std::map<string, float>> getMetersValues();
    vector<std::map<string, vector<float>>> getMetersVectorValues();
    
private:
    void createChannelMetersViews();
   
    vector <shared_ptr<ChannelMetersView>> _channelMetersViews;
    vector<ofxAudioAnalyzerUnit*> _channelAnalyzers;
    vector<ofxAAValue> _enabledValueTypes;
    vector<ofxAABinsValue> _enabledBinsValueTypes;
    

};
