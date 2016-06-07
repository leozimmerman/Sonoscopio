
#include "MetersPanel.h"
#include "ofApp.h"


ofApp* meMainAppPtr;

//----------------------------------------------
#pragma mark - Core Funcs
//----------------------------------------------
void MetersPanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr, vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    meMainAppPtr = dynamic_cast<ofApp*>(appPtr);
    channelAnalyzers = chanAnalyzerPtrs;
    
    setBackgroundColor(ofColor::darkGreen);
    panelColor1 = ofColor::darkSeaGreen;
    panelColor2 = ofColor::darkKhaki;
    
    panelsNum = channelAnalyzers.size();
    
    int panelHeight = _h / panelsNum;
    
    for (int i=0; i<panelsNum; i++){
        
        int y_pos = _y + panelHeight*i;
        ofxAAChannelMetersPanel * p = new ofxAAChannelMetersPanel(_x, y_pos, _w, panelHeight, channelAnalyzers[i]);
       
        if(i%2) p->setMainColor(panelColor2);
        else p->setMainColor(panelColor1);
        
        
        channelPanels.push_back(p);
    }
   
    _panelDir = METERS_SETTINGS_DIR;
    _bDrawFullDisplay = TRUE;
    
}
//----------------------------------------------
void MetersPanel::update(){
    //set meters values from ofxAudioAnalyzerUnit
    for(ofxAAChannelMetersPanel* p : channelPanels){
        p->update();
    }
}


//----------------------------------------------
void MetersPanel::draw(){

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
#pragma mark - Other funcs
//----------------------------------------------
void MetersPanel::reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    
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
        if(i%2) p->setMainColor(panelColor2);
        else p->setMainColor(panelColor1);
        
        channelPanels.push_back(p);
    }
    //------------------------------
   

}
//----------------------------------------------
void MetersPanel::loadSettings(string rootDir){
    
    //string fileName = workingDir+"/"+"metersSettings.xml";
    
    for(int i=0; i<channelPanels.size(); i++){
        string filename = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        channelPanels[i]->loadSettingsFromFile(filename);
    }
}
//----------------------------------------------
void MetersPanel::saveSettings(string rootDir){
    
    for(int i=0; i<channelPanels.size(); i++){
        string filename = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        channelPanels[i]->saveSettingsToFile(filename);
    }
}
//----------------------------------------------
void MetersPanel::adjustPosAndHeight(int y, int h){
    
    _y = y;
    _h = h;
    
    int panelHeight = _h / panelsNum;
    
    for(int i=0; i<channelPanels.size(); i++){
        int y_pos = _y + panelHeight*i;
        channelPanels[i]->adjustPosAndHeight(y_pos, panelHeight);
    }
    
}
//----------------------------------------------
void MetersPanel::setFullDisplay(bool b){
    
    _bDrawFullDisplay = b;
    for (auto chPan : channelPanels){
        chPan->setFullDisplay(_bDrawFullDisplay);
    }
    
}
//----------------------------------------------
vector<std::map<string, float>>& MetersPanel::getMetersValues(){
    
    //??? valuesForOsc declariation here or in header?
    valuesForOsc.clear();
    
    for (int i=0; i<channelPanels.size(); i++){
        
        std::map<string, float> channelMap;
        
        for(ofxAAMeter* m : channelPanels[i]->meters){
           
            if( m->getName()==MTR_NAME_MFCC || m->getName()==MTR_NAME_SPECTRUM ||
               m->getName()==MTR_NAME_HPCP || m->getName()==MTR_NAME_MEL_BANDS){
                
                //???send vector<floats> ??? send binMeters values to OSC?
                
            }else if (m->getName()==MTR_NAME_ONSETS){
                string key =  m->getName();
                ofxAAOnsetMeter* om = dynamic_cast<ofxAAOnsetMeter*>(m);
                channelMap[key] = om->getValue();
                
            }else{
                string key = m->getName();
                channelMap[key]= m->getValue();
            }
            
        }
        valuesForOsc.push_back(channelMap);
    }
    return valuesForOsc;
}