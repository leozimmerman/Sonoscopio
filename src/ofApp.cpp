
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
    ofSetFrameRate(60);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //Time Measurement-----------------
    
    TIME_SAMPLE_SET_FRAMERATE( 60.0f ); //set the app's target framerate (MANDATORY)
    //specify where the widget is to be drawn
    TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );
    TIME_SAMPLE_SET_AVERAGE_RATE(0.1);	//averaging samples, (0..1],
    //1.0 gets you no averaging at all
    //use lower values to get steadier readings
    TIME_SAMPLE_DISABLE_AVERAGE();	//disable averaging
    TIME_SAMPLE_SET_REMOVE_EXPIRED_THREADS(true); //inactive threads will be dropped from the table
    ///TIME_SAMPLE_DISABLE();
    
    //Panels setup------------------
    int mainH = MAIN_PANEL_HEIGHT * ofGetHeight();
    int timeH = TIME_PANEL_HEIGHT * ofGetHeight();
    int meterH = METER_PANEL_HEIGHT * ofGetHeight();
    
    mainPanel.setup(0, 0, ofGetWidth(), mainH, ofGetAppPtr());
    timePanel.setup(0, mainH, ofGetWidth(), timeH, ofGetAppPtr());
   
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    ofAddListener(timePanel.heightResizedEvent, this, &ofApp::onTimelinePanelResize);
    
    //Audio info--------------
    _channels = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    
    //AudioAnalyzer-------------------
    mainAnalyzer.setup(_samplerate, BUFFER_SIZE, _channels);
    metersPanel.setup(0, mainH + timeH, ofGetWidth(), meterH, ofGetAppPtr(), mainAnalyzer.getChannelAnalyzersPtrs());
    
    //OSC sender-----------------------
    _oscHost = "localhost";
    _oscPort = 12345;
    setOscSender(_oscHost, _oscPort);
    

    
    //ofExit();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //analyze soundBuffer----------------
    ofSoundUpdate();
    
    audioMutex.lock();
    
    soundBuffer = timePanel.audioTrack->getCurrentSoundBuffer(BUFFER_SIZE);

    if(timePanel.timeline.getIsPlaying()){
        mainAnalyzer.analyze(soundBuffer);
    }
    
    audioMutex.unlock();
    
    //update panels-------------------
    mainPanel.update();
    timePanel.update();
    metersPanel.update();
    
    
    //send OSC-----------------------
    TS_START("sendOsc");
    sendOscData();
    TS_STOP("sendOsc");
    //--------------------------
    
//    waveform.clear();
//    
//    int ch=0; //channel to visualize
//    
//    for(size_t i = 0; i < soundBuffer.getNumFrames(); i++) {
//        float sample = soundBuffer.getSample(i, ch);
//        float x = ofMap(i, 0, soundBuffer.getNumFrames(), 0, ofGetWidth());
//        float y = ofMap(sample, -1, 1, 0, ofGetHeight());
//        waveform.addVertex(x, y);
//    }

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    TS_START("mainPanel-Draw");
    mainPanel.draw();
    TS_STOP("mainPanel-Draw");
    
    TS_START("metersPanel-Draw");
    metersPanel.draw();
    TS_STOP("metersPanel-Draw");

    TS_START("timePanel-Draw");
    timePanel.draw();
    TS_STOP("timePanel-Draw");
    
    //ofSetColor(255,0,0);
    
    //waveform.draw();

    
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
        sendOscData();
    }
    
    
}
#pragma mark - TimePanel funcs
//--------------------------------------------------------------
void ofApp::openAudioFile(string filename){
    
    audioMutex.lock();
    
    timePanel.openAudioFile(filename);
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    
    _channels = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    
    
    mainAnalyzer.reset(_samplerate, BUFFER_SIZE, _channels);
    metersPanel.reset(mainAnalyzer.getChannelAnalyzersPtrs());
    

    audioMutex.unlock();
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
#pragma mark - OSC funcs
//--------------------------------------------------------------
void ofApp::setOscSender(string host, int port){
    oscSender.setup(host, port);
}
//--------------------------------------------------------------
void ofApp::setOscSenderHost(string host){
    oscSender.setup(host, _oscPort);
}
//--------------------------------------------------------------
void ofApp::setOscSenderPort(int port){
    oscSender.setup(_oscHost, port);
}
//--------------------------------------------------------------
void ofApp::sendOscData(){
    
    ///BUNDLES??
    
    //send Meters values...
    std::map<string, float> metersValues = metersPanel.getMetersValues();
    
    for (auto& kv : metersValues){
        //cout<<kv.first<<" -- "<<kv.second<<endl;
        
        string key = kv.first;
        float floatValue = kv.second;
        
        ofxOscMessage m;
        m.setAddress(key);
        m.addFloatArg(floatValue);
        oscSender.sendMessage(m, false);
        
    }
    
    //send timeline track values...
    std::map<string, float> timelineValues = timePanel.getTracksValues();
    
    for (auto& kv : timelineValues){
        //cout<<kv.first<<" -- "<<kv.second<<endl;
        
        string key = kv.first;
        float floatValue = kv.second;
        
        ofxOscMessage m;
        m.setAddress(key);
        m.addFloatArg(floatValue);
        oscSender.sendMessage(m, false);
        
    }
    

}
//--------------------------------------------------------------
#pragma mark - Settings funcs
//--------------------------------------------------------------
void ofApp::saveSettings(){
    
    timePanel.saveSettings();
    metersPanel.saveSettings();
    ofLogVerbose()<< "ofApp: Settings SAVED";
}
//--------------------------------------------------------------
void ofApp::loadSettings(){

    timePanel.loadSettings();
    metersPanel.loadSettings();
    ofLogVerbose() << "ofApp: Settings LOADED";
}



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
