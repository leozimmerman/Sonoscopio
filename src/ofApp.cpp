
#include "ofApp.h"

#pragma mark - Core funcs
//--------------------------------------------------------------
void ofApp::setup(){
	
    //Gral---------------------------
    ofSetBackgroundColor(100);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //AudioAnalyzer-------------------
    audioAnalyzer.setup(512, 44100);
    
    //Panels setup------------------
    
    mainPanel.setup(0,0, ofGetWidth(), 100, ofGetAppPtr());
    timePanel.setup(0,100, ofGetWidth(), 500, ofGetAppPtr());
    metersPanel.setup(0, 600, ofGetWidth(), ofGetHeight()-600, ofGetAppPtr(), &audioAnalyzer);
    
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    ofAddListener(timePanel.heightResized, this, &ofApp::onTimelinePanelResize);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    
    vector<float> buffer = timePanel.audioTrack->getCurrentBuffer();
//    vector<float> buffer = timePanel.audioTrack->getCurrentBufferForChannel(512, 1);
    
    audioAnalyzer.analyze(&buffer[0], 512);
    
    
    mainPanel.update();
    timePanel.update();
    metersPanel.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    mainPanel.draw();
    timePanel.draw();
    metersPanel.draw();

    
}
//--------------------------------------------------------------
void ofApp::exit(){
    audioAnalyzer.exit();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    timePanel.keyPressed(key);
    /*
    'e' expands focused track
    'd' enables/disables focused track
    'a' adjust tracks height shorcut
    */
    
    switch (key) {

    }
    
    
}
#pragma mark - Other funcs
//--------------------------------------------------------------
void ofApp::openAudioFile(string filename){

    
    timePanel.openAudioFile(filename);
    mainPanel.setFileInfoString(timePanel.getFileInfo());

}

//--------------------------------------------------------------
void ofApp::startStopPlaying(){

    timePanel.startStopPlaying();
}
//--------------------------------------------------------------
void ofApp::onTimelinePanelResize(int &h){

    //cout << "ofAPP:" << h << endl;
    
    //new metersPanel vars
    int new_y = mainPanel.getHeight() + h;
    int new_h = ofGetHeight() - new_y;
    
    metersPanel.adjustPosAndHeight(new_y, new_h);
    
    
}
//--------------------------------------------------------------
///**************************************************************
#pragma mark - OF core other tools
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
