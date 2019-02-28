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
    for(auto ch : _channelMetersViews){
        ch->update();
    }
}

void MetersView::draw(){
    View::draw();
    for(auto ch : _channelMetersViews){
        ch->draw();
    }
}

void MetersView::exit(){
    for (auto ch : _channelMetersViews){
        ch->exit();
    }
    _channelMetersViews.clear();
}

void MetersView::scrollUp(){
    for (auto ch : _channelMetersViews) {
        ch->scrollUp();
    }
}

void MetersView::scrollDown(){
    for (auto ch : _channelMetersViews) {
        ch->scrollDown();
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

void MetersView::setClicksEnabled(bool enabled){
    View::setClicksEnabled(enabled);
    for (auto ch : _channelMetersViews) {
        ch->setClicksEnabled(enabled);
    }
}

#pragma mark - Settings

void MetersView::setStateFromSettings(MetersPanelSettings& settings){
    if (_channelMetersViews.size() != settings.channelMeters.size()){
        return;
    }
    
    for (int i=0; i<_channelMetersViews.size(); i++){
        _channelMetersViews[i]->setStateFromSettings(settings.channelMeters[i]);
    }
}

void MetersView::loadStateIntoSettings(MetersPanelSettings* settings){
    settings->channelMeters.clear();
    settings->channelMeters.shrink_to_fit();
    
    for (auto ch : _channelMetersViews){
        ChannelMeterSettings ch_s;
        ch->loadStateIntoSettings(&ch_s);
        settings->channelMeters.push_back(ch_s);
    }
}

#pragma mark - Meters Values Getters

vector<std::map<string, float>> MetersView::getMetersValues(){
    vector < std::map<string, float> > singleValues;
    for (auto ch : _channelMetersViews){
        std::map<string, float> channelMap =  ch->getMetersValues();
        singleValues.push_back(channelMap);
    }
    return singleValues;
}

vector<std::map<string, vector<float>>> MetersView::getMetersVectorValues(){
    vector < std::map<string, vector<float> > > vectorValues;
    for (auto ch : _channelMetersViews){
        std::map<string, vector<float>> channelMap = ch->getMetersVectorValues();
        vectorValues.push_back(channelMap);
    }
    return vectorValues;
}


