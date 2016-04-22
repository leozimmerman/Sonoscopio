

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
    //Gral---------------------------
    ofBackground(0);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //Panels setup------------------
    
    mainPanel.setup(0,0, ofGetWidth(), 100, ofGetAppPtr());
    timePanel.setup(0,100, ofGetWidth(), 500, ofGetAppPtr());
    
    
    mainPanel.setFileInfoString(timePanel.getFileInfo());


}

//--------------------------------------------------------------
void ofApp::update(){
    

//    vector<float> buffer = track->getCurrentBufferForChannel(512, 1);
//    
//    int bufferSize = buffer.size();
//    waveform.clear();
//    for(size_t i = 0; i < buffer.size(); i++) {
//        float sample = buffer[i];
//        float x = ofMap(i, 0, buffer.size(), 0, ofGetWidth());
//        float y = ofMap(sample, -1, 1, 0, ofGetHeight());
//        waveform.addVertex(x, y);
//    }
//
    
    mainPanel.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
   
    //waveform.draw();
    
    mainPanel.draw();
    timePanel.draw();

    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    switch (key) {

    }
    
    
}
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
