//
//  MetersView.cpp
//  Sonoscopio
//
//  Created by Leo on 07/09/2018.
//

#include "MetersView.h"
void MetersView::setup(int x, int y, int w, int h){
    View::setup(x, y, w, h);
    setBackgroundColor(ofColor::red);
    panelColor1 = COLOR_MAIN_A;
    panelColor2 = COLOR_MAIN_B;
    _bDrawFullDisplay = TRUE;
}

void MetersView::setupChannelMeters(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs) {
    channelAnalyzers = chanAnalyzerPtrs;
    
    int panelsNum = channelAnalyzers.size();
    int panelHeight = (_h) / panelsNum;
    
    //FIXME: Codigo repetido
    for (int i=0; i<panelsNum; i++){
        int y_pos = _y + panelHeight*i;
        int panelId = i;
        
        auto p = make_shared<ChannelMetersView>(_x, y_pos, _w, panelHeight, panelId, channelAnalyzers[i]);
        
        if(i%2) p->setMainColor(panelColor2);
        else p->setMainColor(panelColor1);
        
        channelPanels.push_back(p);
    }
}
//----------------------------------------------
void MetersView::update(){
    for(auto p : channelPanels){
        p->update();
    }
}
//----------------------------------------------
void MetersView::draw(){
    View::draw();
    for(auto p : channelPanels){
        p->draw();
    }
}
//----------------------------------------------
void MetersView::exit(){
    for (auto p : channelPanels){
        p->exit();
    }
    channelPanels.clear();
}
//----------------------------------------------
//TODO: Move scroll funcs to ChannelPannelMV
void MetersView::scrollUp(){
    for (auto p : channelPanels) {
        p->scrollUp();
    }
}
//----------------------------------------------
void MetersView::scrollDown(){
    for (auto p : channelPanels) {
        p->scrollDown();
    }
}
//----------------------------------------------
void MetersView::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    for(int i=0; i<channelPanels.size(); i++){
        int width = w / channelPanels.size();
        int xpos = i * width;
        channelPanels[i]->resize(xpos, y, width, h);
    }
}
//----------------------------------------------
void MetersView::reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    for (auto p : channelPanels){
        p->exit();
    }
    channelPanels.clear();
    
    channelAnalyzers = chanAnalyzerPtrs;
    panelsNum = channelAnalyzers.size();
    
    int panelHeight = (_h) / panelsNum;
    //FIXME: Codigo repetido
    for (int i=0; i<panelsNum; i++){
        int y_pos = _y + panelHeight*i;
        int panelId = i;
        auto p = make_shared<ChannelMetersView>(_x, y_pos, _w, panelHeight, panelId, channelAnalyzers[i]);
        if(i%2) p->setMainColor(panelColor2);
        else p->setMainColor(panelColor1);
        
        channelPanels.push_back(p);
    }
    
}

//----------------------------------------------
vector<std::map<string, float>>& MetersView::getMetersValues(){
    
    singleValuesForOsc.clear();
    
    for (int i=0; i<channelPanels.size(); i++){
        
        std::map<string, float> channelMap;
        
        for(MeterView* m : channelPanels[i]->meters){
            
            if (m->getName()==MTR_NAME_ONSETS){
                
                string key =  m->getName();
                OnsetMeterView* om = dynamic_cast<OnsetMeterView*>(m);
                channelMap[key] = om->getValue();
                
            }else if(m->getName()!= MTR_NAME_MFCC && m->getName()!= MTR_NAME_SPECTRUM &&
                     m->getName()!= MTR_NAME_HPCP && m->getName()!= MTR_NAME_MEL_BANDS &&
                     m->getName()!= MTR_NAME_TRISTIMULUS){
                string key = m->getName();
                channelMap[key]= m->getValue();
            }
            
        }
        singleValuesForOsc.push_back(channelMap);
    }
    return singleValuesForOsc;
}
//----------------------------------------------
vector<std::map<string, vector<float>>>& MetersView::getMetersVectorValues(){
    
    vectorValuesForOsc.clear();
    
    for (int i=0; i<channelPanels.size(); i++){
        
        std::map<string, vector<float>> channelMap;
        
        channelMap[MTR_NAME_MEL_BANDS] = channelPanels[i]->getMelBandsValues();
        channelMap[MTR_NAME_MFCC] = channelPanels[i]->getMfccValues();
        channelMap[MTR_NAME_HPCP] = channelPanels[i]->getHpcpValues();
        channelMap[MTR_NAME_TRISTIMULUS] = channelPanels[i]->getTristimulusValues();
        
        vectorValuesForOsc.push_back(channelMap);
    }
    
    return vectorValuesForOsc;
}
