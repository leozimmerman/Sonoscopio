
#include "ofApp.h"

 //ofxAudioAnalyzer audioAnalyzer(44100, 512);

#pragma mark - Core funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    ///HAY UN BACK EN EL ANALYZER, MIRAR POWER VALUES CADA 4 FRAMES
	
    //Gral---------------------------
    ofSetBackgroundColor(100);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //AudioAnalyzer-------------------
    //audioAnalyzer.setup(512, 44100);
    mainAnalyzer.setup(44100, 512, 1);///channels!
    
    //Panels setup------------------
    
    mainPanel.setup(0,0, ofGetWidth(), 100, ofGetAppPtr());
    timePanel.setup(0,100, ofGetWidth(), 500, ofGetAppPtr());
    metersPanel.setup(0, 600, ofGetWidth(), ofGetHeight()-600, ofGetAppPtr(), mainAnalyzer.getChannelAnalyzersPtrs());
    
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    ofAddListener(timePanel.heightResizedEvent, this, &ofApp::onTimelinePanelResize);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    
    vector<float> buffer = timePanel.audioTrack->getCurrentBuffer();
    //vector<float> buffer1 = timePanel.audioTrack->getCurrentBufferForChannel(512, 1);
    //vector<float> buffer2 = timePanel.audioTrack->getCurrentBufferForChannel(512, 2);
    
//    ofSoundBuffer testBuffer1, testBuffer2;
//    
//    testBuffer1.setNumChannels(1);
//    testBuffer1.setSampleRate(44100);
//    testBuffer1.copyFrom(buffer1, 1, 44100);
//    
//    testBuffer2.setNumChannels(2);
//    testBuffer2.setSampleRate(44100);
//    testBuffer2.copyFrom(buffer2, 1, 44100);
    

    
    //---------------------------
    ofSoundBuffer soundBuffer;

    soundBuffer.copyFrom(buffer, 1, 44100);///channels!
    //buffer.setChannel(testBuffer1, 1);
    //buffer.setChannel(testBuffer2, 2);
    //cout<<buffer.getNumChannels()<<endl;
    mainAnalyzer.analyze(soundBuffer);
    
    //--------------------------
    
    
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
    mainAnalyzer.exit();
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
