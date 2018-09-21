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
    void setupChannelMeters(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    
    void draw() override;
    void update();
    void exit();
    
    void resize(int x, int y, int w, int h) override;
    
    void reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs);
    
    void scrollUp();
    void scrollDown();
    
    vector<std::map<string, float>>& getMetersValues();
    vector<std::map<string, vector<float>>>& getMetersVectorValues();

    vector <ChannelMetersView*> channelPanels;
    ofColor panelColor1, panelColor2;
    int panelsNum;
    bool _bDrawFullDisplay;
    vector<ofxAudioAnalyzerUnit*> channelAnalyzers;

    
private:
    vector < std::map<string, float> > singleValuesForOsc;
    vector < std::map<string, vector<float> > > vectorValuesForOsc;
};
