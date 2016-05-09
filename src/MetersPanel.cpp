
#include "MetersPanel.h"
#include "ofApp.h"


ofApp* meMainAppPtr;

#pragma mark - Core Funcs

void MetersPanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr, vector<ofxAudioAnalyzer*>& chanAnalyzerPtrs){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    meMainAppPtr = dynamic_cast<ofApp*>(appPtr);
    channelAnalyzers = chanAnalyzerPtrs;
    
    setBackgroundColor(ofColor::darkGreen);
    
    panelsNum = channelAnalyzers.size();
    
    int panelHeight = _h / panelsNum;
    
    for (int i=0; i<panelsNum; i++){
        
        int y_pos = _y + panelHeight*i;
        ofxAAChannelMetersPanel * p = new ofxAAChannelMetersPanel(_x, y_pos, _w, panelHeight, channelAnalyzers[i]);
        channelPanels.push_back(p);
    }
    
   

    
}

//----------------------------------------------

void MetersPanel::update(){
    //set meters values from ofxAudioAnalyzer
    for(ofxAAChannelMetersPanel* p : channelPanels){
        p->update();
    }
    //chPanel.update();

}


//----------------------------------------------
void MetersPanel::draw(){
    
    //background
    ofPushStyle();
        ofSetColor(getBackgroundColor());
        ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    //panels
    //chPanel.draw();
    for(ofxAAChannelMetersPanel* p : channelPanels){
        p->draw();
    }
    


}
//----------------------------------------------
void MetersPanel::exit(){
    for (ofxAAChannelMetersPanel* p : channelPanels){
        p->exit();
    }
    channelPanels.clear();

}
//----------------------------------------------
void MetersPanel::reset(vector<ofxAudioAnalyzer*>& chanAnalyzerPtrs){
    
    
    for (ofxAAChannelMetersPanel* p : channelPanels){
        p->exit();
    }
    channelPanels.clear();
    
    //--------------------------
    
    channelAnalyzers = chanAnalyzerPtrs;
    panelsNum = channelAnalyzers.size();
    
    int panelHeight = _h / panelsNum;
    
    for (int i=0; i<panelsNum; i++){
        
        int y_pos = _y + panelHeight*i;
        ofxAAChannelMetersPanel * p = new ofxAAChannelMetersPanel(_x, y_pos, _w, panelHeight, channelAnalyzers[i]);
        channelPanels.push_back(p);
        
    }

}
//----------------------------------------------
void MetersPanel::adjustPosAndHeight(int y, int h){
    
    _y = y;
    _h = h;
    
    //chPanel.adjustPosAndHeight(_y, _h);
    
    int panelHeight = _h / panelsNum;
    
    for(int i=0; i<channelPanels.size(); i++){
        int y_pos = _y + panelHeight*i;
        channelPanels[i]->adjustPosAndHeight(y_pos, panelHeight);
    }
    
}
//----------------------------------------------