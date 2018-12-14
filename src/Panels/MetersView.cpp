//
//  MetersView.cpp
//  Sonoscopio
//
//  Created by Leo on 07/09/2018.
//

#include "MetersView.h"

#pragma mark - Core funcs

void MetersView::setup(int x, int y, int w, int h){
    View::setup(x, y, w, h);
    setBackgroundColor(ofColor::darkGray);
}

void MetersView::setupChannelMeters(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs, vector<ofxAAAlgorithmType> enabledAlgorithms) {
    _channelAnalyzers = chanAnalyzerPtrs;
    _enabledAlgorithmTypes = enabledAlgorithms;
    createChannelMetersViews();
}

void MetersView::setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms){
    _enabledAlgorithmTypes = enabledAlgorithms;
    for(auto p : _channelMetersViews){
        p->setEnabledAlgorithms(enabledAlgorithms);
    }
}

void MetersView::reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    for (auto p : _channelMetersViews){
        p->exit();
    }
    _channelMetersViews.clear();
    _channelAnalyzers = chanAnalyzerPtrs;
    createChannelMetersViews();
}

void MetersView::createChannelMetersViews() {
    u_long panelsNum = _channelAnalyzers.size();
    int panelHeight = (_h) / panelsNum;
    
    for (int i=0; i<panelsNum; i++){
        int y_pos = _y + panelHeight*i;
        int panelId = i;
        ofColor mainColor = (i%2) ? COLOR_MAIN_B : COLOR_MAIN_A;
        auto p = make_shared<ChannelMetersView>(_x, y_pos, _w, panelHeight, panelId, _channelAnalyzers[i], _enabledAlgorithmTypes, mainColor);
        _channelMetersViews.push_back(p);
    }
}

void MetersView::update(){
    for(auto p : _channelMetersViews){
        p->update();
    }
}

void MetersView::draw(){
    View::draw();
    for(auto p : _channelMetersViews){
        p->draw();
    }
}

void MetersView::exit(){
    for (auto p : _channelMetersViews){
        p->exit();
    }
    _channelMetersViews.clear();
}

void MetersView::scrollUp(){
    for (auto p : _channelMetersViews) {
        p->scrollUp();
    }
}

void MetersView::scrollDown(){
    for (auto p : _channelMetersViews) {
        p->scrollDown();
    }
}

void MetersView::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    for(int i=0; i<_channelMetersViews.size(); i++){
        int width = w / _channelMetersViews.size();
        int xpos = i * width;
        _channelMetersViews[i]->resize(xpos, y, width, h);
    }
}

#pragma mark - Settings

void MetersView::loadSettings(MetersPanelSettings& settings){    
    if (_channelMetersViews.size() != settings.channelMeters.size()){
        return;
    }
    
    for (int i=0; i<_channelMetersViews.size(); i++){
        _channelMetersViews[i]->loadSettings(settings.channelMeters[i]);
    }
}

void MetersView::updateCurrentSettings(){
    currentChannelSettings.clear();
    for (auto channelView : _channelMetersViews){
        channelView->updateCurrentSettings();
        currentChannelSettings.push_back(channelView->getCurrentSettingsRef());
    }
}

#pragma mark - Meters Values Getters

vector<std::map<string, float>> MetersView::getMetersValues(){
    vector < std::map<string, float> > singleValuesForOsc;
    for (auto ch : _channelMetersViews){
        std::map<string, float> channelMap =  ch->getMetersValues();
        singleValuesForOsc.push_back(channelMap);
    }
    return singleValuesForOsc;
}

vector<std::map<string, vector<float>>> MetersView::getMetersVectorValues(){
    vector < std::map<string, vector<float> > > vectorValuesForOsc;
    for (auto ch : _channelMetersViews){
        std::map<string, vector<float>> channelMap = ch->getMetersVectorValues();
        vectorValuesForOsc.push_back(channelMap);
    }
    return vectorValuesForOsc;
}


