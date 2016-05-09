
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
    
  
    
    
    //Panels setup------------------
    
    mainPanel.setup(0,0, ofGetWidth(), 100, ofGetAppPtr());
    timePanel.setup(0,100, ofGetWidth(), 500, ofGetAppPtr());
   
    
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    ofAddListener(timePanel.heightResizedEvent, this, &ofApp::onTimelinePanelResize);
    
    //----
    _channels = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    
    //AudioAnalyzer-------------------
    mainAnalyzer.setup(_samplerate, BUFFER_SIZE, _channels);///channels!
    metersPanel.setup(0, 600, ofGetWidth(), ofGetHeight()-600, ofGetAppPtr(), mainAnalyzer.getChannelAnalyzersPtrs());
    
    
    _oscHost = "localhost";
    _oscPort = 12345;
    setOscSender(_oscHost, _oscPort);
    
}
ofPolyline waveform;
//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    // "lastBuffer" is shared between update() and audioOut(), which are called
    // on two different threads. This lock makes sure we don't use lastBuffer
    // from both threads simultaneously (see the corresponding lock in audioOut())
    unique_lock<mutex> lock(audioMutex);
    
    ofSoundUpdate();
//    vector<float> buffer1 = timePanel.audioTrack->getCurrentBufferForChannel(512, 0);
//    vector<float> buffer2 = timePanel.audioTrack->getCurrentBufferForChannel(512, 1);
    
    
   soundBuffer = timePanel.audioTrack->getCurrentSoundBuffer(BUFFER_SIZE);

//    cout<<"SB info--"<<endl;
//    cout<<"SB samplerate: "<<soundBuffer.getSampleRate()<<endl;
//    cout<<"SB buffersize: "<<soundBuffer.getNumFrames()<<endl;
//    cout<<"SB channels: "<<soundBuffer.getNumChannels()<<endl;
    
    if(timePanel.timeline.getIsPlaying()){
        mainAnalyzer.analyze(soundBuffer);
    }
    
    //--------------------------
    
    
    mainPanel.update();
    timePanel.update();
    metersPanel.update();
    
    
    //--------------------------
    
    waveform.clear();
    
    int ch=0; //channel to visualize
    
    for(size_t i = 0; i < soundBuffer.getNumFrames(); i++) {
        float sample = soundBuffer.getSample(i, ch);
        float x = ofMap(i, 0, soundBuffer.getNumFrames(), 0, ofGetWidth());
        float y = ofMap(sample, -1, 1, 0, ofGetHeight());
        waveform.addVertex(x, y);
    }

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    mainPanel.draw();
    timePanel.draw();
    metersPanel.draw();
    
    ofSetColor(255,0,0);
    waveform.draw();

    
}
//--------------------------------------------------------------
void ofApp::exit(){
    mainAnalyzer.exit();
    metersPanel.exit();
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
    
    if(key == 'a' || key == 'A'){
        ofxOscMessage m;
        m.setAddress("/test");
        m.addIntArg(1);
        m.addFloatArg(3.5f);
        m.addStringArg("hello");
        m.addFloatArg(ofGetElapsedTimef());
        oscSender.sendMessage(m, false);
    }
    
    
}
#pragma mark - Other funcs
//--------------------------------------------------------------
void ofApp::openAudioFile(string filename){
    
    ///THREADED????
    
    timePanel.openAudioFile(filename);
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    
    _channels = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    
    
    mainAnalyzer.reset(_samplerate, BUFFER_SIZE, _channels);
    ofLogVerbose()<<"mainAnalyzer RESETED";
    metersPanel.reset(mainAnalyzer.getChannelAnalyzersPtrs());
    ofLogVerbose()<<"metesPanel RESETD";
    

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
void ofApp::setOscSender(string host, int port){
    oscSender.setup(host, port);
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
