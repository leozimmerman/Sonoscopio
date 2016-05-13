
#include "ofApp.h"

//TODO: Comment addon funcs
//TODO: Add ofxAudioAnalyzer constants configuration (setters & getters)

//TODO: Change github repo

//FIXME: HAY UN BUG EN EL ANALYZER, MIRAR POWER VALUES CADA 4 FRAMES
//FIXME: draw performance - FPS




#pragma mark - Core funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    //Gral---------------------------
    ofSetBackgroundColor(ofColor::black);
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
    TIME_SAMPLE_DISABLE();
    
    //Panels setup------------------
    _currentAnalysisMode = SPLIT;
    _bufferSize = 512;
    
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
    mainAnalyzer.setup(_samplerate, _bufferSize, _channels);
    metersPanel.setup(0, mainH + timeH, ofGetWidth(), meterH, ofGetAppPtr(), mainAnalyzer.getChannelAnalyzersPtrs());
    
    //OSC sender-----------------------
    _oscHost = "localhost";
    _oscPort = 12345;
    setOscSender(_oscHost, _oscPort);
    _bSendOsc = TRUE;
    
    //-------------------------------
    _projectDir = "";
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //analyze soundBuffer----------------
    ofSoundUpdate();
    
    audioMutex.lock();
    
    TS_START("GET-AUDIO-BUFFERS");
    if(_currentAnalysisMode==SPLIT){
        soundBuffer = timePanel.audioTrack->getCurrentSoundBuffer(_bufferSize);//multichannel soundbuffer
    }else if(_currentAnalysisMode==MONO){
        soundBuffer = timePanel.audioTrack->getCurrentSoundBufferMono(_bufferSize);//mono soundbuffer
    }
    TS_STOP("GET-AUDIO-BUFFERS");
    
    
    TS_START("AUDIO-ANALYSIS");
    if(timePanel.timeline.getIsPlaying()){
      mainAnalyzer.analyze(soundBuffer);
    }
    TS_STOP("AUDIO-ANALYSIS");
    
    audioMutex.unlock();
    
    //update panels-------------------
    TS_START("PANELS-UPDATE");
    mainPanel.update();
    timePanel.update();
    metersPanel.update();
    TS_STOP("PANELS-UPDATE");
    
    //send OSC-----------------------
    TS_START("SEND-OSC");
    if(_bSendOsc) sendOscData();
    TS_STOP("SEND-OSC");
    //--------------------------
    
//    waveform.clear();
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
    
    
    
    TS_START("METERS-PANEL");
    metersPanel.draw();
    TS_STOP("METERS-PANEL");

    TS_START("TIMELINE-PANEL");
    timePanel.draw();
    TS_STOP("TIMELINE-PANEL");
    
    TS_START("MAIN-PANEL");
    mainPanel.draw();
    TS_STOP("MAIN-PANEL");
    
    
    
//    ofPushStyle();
//    ofSetColor(255,0,0);
//    waveform.draw();
//    ofPopStyle();
    
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
    /*
     'm': time measurement on/off
     */
    switch (key) {
        case 'm':
            if(TIME_SAMPLE_GET_ENABLED()) TIME_SAMPLE_DISABLE();
            else TIME_SAMPLE_ENABLE();
            break;
    }
    
   
    
    if(key == 'a' || key == 'A'){
        //sendOscData();
        //openProject();
    }
    
    
}
#pragma mark - Audio Engine funcs
//--------------------------------------------------------------
void ofApp::openAudioFile(string filename){
    
    
    audioMutex.lock();
    
    timePanel.openAudioFile(filename);
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    
    _channels = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    
    resetAnalysisEngine();

    audioMutex.unlock();
    
}

//--------------------------------------------------------------
void ofApp::resetAnalysisEngine(){
    
    if(_currentAnalysisMode == SPLIT){
        
        mainAnalyzer.reset(_samplerate, _bufferSize, _channels);
        metersPanel.reset(mainAnalyzer.getChannelAnalyzersPtrs());
        
    }else if(_currentAnalysisMode == MONO){
        
        mainAnalyzer.reset(_samplerate, _bufferSize, 1);
        metersPanel.reset(mainAnalyzer.getChannelAnalyzersPtrs());
        
    }
    
}
//--------------------------------------------------------------
void ofApp::setAnalysisMode(analysisMode mode){
    
    audioMutex.lock();
    
    _currentAnalysisMode = mode;
    resetAnalysisEngine();
    
    audioMutex.unlock();
}
//--------------------------------------------------------------
void ofApp::setBufferSize(int bs){
    
    audioMutex.lock();
    
    _bufferSize = bs;
    resetAnalysisEngine();
    
    audioMutex.unlock();
    
}
//--------------------------------------------------------------
#pragma mark - TimePanel funcs
//--------------------------------------------------------------
void ofApp::startStopPlaying(){

    timePanel.startStopPlaying();
}
//--------------------------------------------------------------
void ofApp::onTimelinePanelResize(int &h){
    
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
    
    //???ADD OSC BUNDLES?
    
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
    
    //TODO: Send Vector Algorithm values
    

}
//--------------------------------------------------------------
#pragma mark - Settings funcs
//--------------------------------------------------------------
void ofApp::openProject(string projectDir ){
   
    ofLogVerbose()<<"ofApp Opening project in :"<<projectDir<<endl;
    
    _projectDir = projectDir + "/";
    
    //Check if project is correct----------------------------------
    
    ofDirectory dir(_projectDir);
    
    //check settings folders
   
    if(!dir.doesDirectoryExist(_projectDir + "main_settings")){
        ofLogError()<< "ofApp openProject: No main_settings folder found in the project directory.";
        return;
    }else if(!dir.doesDirectoryExist(_projectDir + "meters_settings")){
        ofLogError()<< "ofApp openProject: No meters_settings folder found in the project directory.";
        return;
    }else if(!dir.doesDirectoryExist(_projectDir + "timeline_settings")){
        ofLogError()<< "ofApp openProject: No timeline_settings folder found in the project directory.";
        return;
    }
    
    //check audiofile:
    dir.allowExt("wav");
    dir.allowExt("mp3");
    //populate the directoryr
    dir.listDir();
    
    //check there is at least and only one file
    if(dir.size() == 0){
        ofLogError()<< "ofApp openProject: No audio file found in the project directory. Only wav and mp3 extensions.";
        return;
    }else if(dir.size()>1){
        ofLogError()<< "ofApp openProject: More than one audio file found in project directory. Must be only one.";
        return;
    }
    
    //check name
    string audioFileName;
    
    if(dir.getPath(0) == _projectDir+"audiofile.wav" || dir.getPath(0) == _projectDir+"audiofile.mp3"){
        audioFileName = dir.getPath(0);//set audiofile name
    }else{
        ofLogError()<< "ofApp openProject: Audio file name is incorrect. Must be named audiofile.wav or audiofile.mp3";
        return;
    }
    //-------------------------------------------------------
    
    openAudioFile(audioFileName);
    
    mainPanel.loadSettings(_projectDir);
    timePanel.loadSettings(_projectDir);
    metersPanel.loadSettings(_projectDir);
}
//--------------------------------------------------------------
//??? Add closeProject?
//--------------------------------------------------------------
void ofApp::saveSettings(){
    
    mainPanel.saveSettings(_projectDir);
    timePanel.saveSettings(_projectDir);
    metersPanel.saveSettings(_projectDir);
    
    if(_projectDir=="")
        ofLogVerbose() << "ofApp: Settings SAVED to data/";
    else
        ofLogVerbose() << "ofApp: Settings SAVED to: "<<_projectDir;
}
//--------------------------------------------------------------
void ofApp::loadSettings(){
    
    mainPanel.loadSettings(_projectDir);
    timePanel.loadSettings(_projectDir);
    metersPanel.loadSettings(_projectDir);
    
    if(_projectDir=="")
        ofLogVerbose() << "ofApp: Settings LOADED from data/";
    else
        ofLogVerbose() << "ofApp: Settings LOADED from: "<<_projectDir;
}
//--------------------------------------------------------------
#pragma mark - Other funcs


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
