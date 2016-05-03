
#include "MetersPanel.h"
#include "ofApp.h"


ofApp* meMainAppPtr;

#pragma mark - Core Funcs

void MetersPanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr, ofxAudioAnalyzer* aaPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    meMainAppPtr = dynamic_cast<ofApp*>(appPtr);
    audioAnalyzerPtr = aaPtr;
    
    setBackgroundColor(ofColor::darkGreen);
    
    chPanel.setup(_x, _y, _w, _h, aaPtr);
   

    
}

//----------------------------------------------

void MetersPanel::update(){
    //set meters values from ofxAudioAnalyzer
    chPanel.update();

}


//----------------------------------------------
void MetersPanel::draw(){
    
    //background
    ofPushStyle();
        ofSetColor(getBackgroundColor());
        ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    //panels
    chPanel.draw();
    


}
//----------------------------------------------
void MetersPanel::adjustPosAndHeight(int y, int h){
    
    _y = y;
    _h = h;
    
    chPanel.adjustPosAndHeight(_y, _h);
    
}
//----------------------------------------------