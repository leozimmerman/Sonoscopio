
//**********************************************************************

//-: MAIN PANEL:
//FIXME: BMP grid not working?

//-: TIMELINE PANEL:


//-: METERS PANEL:


//-: GRAL:
//FIXME: avoid reseting engine when playing&analyzing ????




//**********************************************************************

#include "ofApp.h"

#pragma mark - Core funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    //Gral---------------------------
    ofSetBackgroundColor(ofColor::black);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetFrameRate(FRAME_RATE);
    _frameRate = FRAME_RATE;
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //Time Measurement-----------------
    
    TIME_SAMPLE_SET_FRAMERATE( FRAME_RATE ); //set the app's target framerate (MANDATORY)
    //specify where the widget is to be drawn
    TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );
    TIME_SAMPLE_SET_AVERAGE_RATE(0.1);	//averaging samples, (0..1],
    //1.0 gets you no averaging at all
    //use lower values to get steadier readings
    TIME_SAMPLE_DISABLE_AVERAGE();	//disable averaging
    TIME_SAMPLE_SET_REMOVE_EXPIRED_THREADS(true); //inactive threads will be dropped from the table
    TIME_SAMPLE_DISABLE();
    
    //Panels setup------------------
    _currentAnalysisMode = MONO;
    _bufferSize = 512;
    
    int mainH = MAIN_PANEL_HEIGHT * ofGetHeight();
    int timeH = TIME_PANEL_HEIGHT * ofGetHeight();
    int metersH = METER_PANEL_HEIGHT * ofGetHeight();
    
    mainPanel.setup(0, 0, ofGetWidth(), mainH, ofGetAppPtr());
    timePanel.setup(0, mainH, ofGetWidth(), timeH, ofGetAppPtr());
   
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    ofAddListener(timePanel.heightResizedEvent, this, &ofApp::onTimelinePanelResize);
    
    //Audio info--------------
    _channelsNum = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    _totalFramesNum = timePanel.timeline.getDurationInFrames();
    
    //AudioAnalyzer-------------------
    int this_channelNum;
    if(_currentAnalysisMode == SPLIT){
        this_channelNum =  _channelsNum;
    }else if(_currentAnalysisMode == MONO){
        this_channelNum = 1;
    }
    
    mainAnalyzer.setup(_samplerate, _bufferSize, this_channelNum);
    metersPanel.setup(0, mainH + timeH, ofGetWidth(), metersH, ofGetAppPtr(), mainAnalyzer.getChannelAnalyzersPtrs());
    
    //OSC sender-----------------------
    _oscHost = "localhost";
    _oscPort = 12345;
    setOscSender(_oscHost, _oscPort);
    _bSendOsc = TRUE;
    
    //-------------------------------
    _projectDir = "";
    
    //----------------------------
    dataSaver.setup(ofGetAppPtr());
    
    verdana.load("gui_assets/fonts/verdana.ttf", 25, false, false);
    
    //adjust timePanel Height
    timePanel.checkIfHeightChanged();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle("Sonoscopio - " + ofToString(ofGetFrameRate(),2));
    
    //if dataSaver thread is running do not update anything and wait for it to finish
    if(dataSaver.isThreadRunning()){
        return;
    }
    
    //**********************************************************************
    
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
    if(timePanel.getIfIsDragging() == false) metersPanel.update();
    TS_STOP("PANELS-UPDATE");
    
    //send OSC-----------------------
    TS_START("SEND-OSC");
    if(_bSendOsc) sendOscData();
    TS_STOP("SEND-OSC");
    //--------------------------
    
    
    
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
    
    //saving data sign
    if(dataSaver.isThreadRunning()){
        drawSavingAnalysisSign();
    }
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    
    mainAnalyzer.exit();
    metersPanel.exit();
    
    dataSaver.stop();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //---------------------------
    //If any dat gui text input is in focus return
    if(mainPanel.getFocused() || timePanel.getFocused() || metersPanel.getFocused()){
        return;
    }
    //---------------------------
    /*
     * 'e' expands focused track
     * 'd' enables/disables focused track
     * 'a' adjust tracks height shorcut
     */
    timePanel.keyPressed(key);
    
    //--------------------------------
    /*
     * 'm': time measurement on/off
     * 'g' Data Saver Starts
     * 'h' Data Saver Stops
     */
    switch (key) {
        
        case 'm':
            if(TIME_SAMPLE_GET_ENABLED()) TIME_SAMPLE_DISABLE();
            else TIME_SAMPLE_ENABLE();
            break;
        
//        case 'g':
//            dataSaver.start();
//            break;
//            
//        case 'h':
//            dataSaver.stop();
//            break;
            
//        case 't':
//            timePanel.addMarker();
//            break;
//            
//        case 'y':
//            timePanel.clearMarkers();
//            break;
        
        case 'f':
            ofToggleFullscreen();
            break;
            
        default:
            break;
    }
    
    
    
}
#pragma mark - Audio Engine funcs
//--------------------------------------------------------------
void ofApp::openAudioFile(string filename){
    
    
    audioMutex.lock();
    
    timePanel.openAudioFile(filename);
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    
    _channelsNum = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    
    resetAnalysisEngine();
    
    dataSaver.reset();

    audioMutex.unlock();
    
}

//--------------------------------------------------------------
void ofApp::resetAnalysisEngine(){
    
    int this_channelNum;
    if(_currentAnalysisMode == SPLIT){
        this_channelNum =  _channelsNum;
    }else if(_currentAnalysisMode == MONO){
        this_channelNum = 1;
    }
    
    mainAnalyzer.reset(_samplerate, _bufferSize, this_channelNum);
    metersPanel.reset(mainAnalyzer.getChannelAnalyzersPtrs());
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
void ofApp::setFrameRate(int fps){
    
    
    _frameRate = fps;
    ofSetFrameRate(_frameRate);
    timePanel.setFrameRate(_frameRate);
    
    //update file info frames num info:
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    
    ofLogVerbose()<<"Frame Rate changed to: "<<ofToString(fps);
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
    
    vector<std::map<string, float>> metersValues = metersPanel.getMetersValues();
    
    for(int i=0; i<metersValues.size(); i++){
        
        ofxOscMessage m;
        
        //address: "/ch0" - "/ch1" - "/ch2" etc...
        string address = "/ch" + ofToString(i);
        m.setAddress(address);
        
        //sames order as Osc Indexes (ofxAudioAnalyzerAlgorithms.h)
        m.addFloatArg(metersValues[i].at(MTR_NAME_POWER));//0
        m.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_FREQ));//1
        m.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_CONF));//2
        m.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_SALIENCE));//3
        m.addFloatArg(metersValues[i].at(MTR_NAME_HFC));//4
        m.addFloatArg(metersValues[i].at(MTR_NAME_CENTROID));//5
        m.addFloatArg(metersValues[i].at(MTR_NAME_SPEC_COMP));//6
        m.addFloatArg(metersValues[i].at(MTR_NAME_INHARMONICTY));//7
        m.addFloatArg(metersValues[i].at(MTR_NAME_DISSONANCE));//8
        m.addFloatArg(metersValues[i].at(MTR_NAME_ROLL_OFF));//9
        m.addFloatArg(metersValues[i].at(MTR_NAME_ODD_TO_EVEN));//10
        m.addInt32Arg(metersValues[i].at(MTR_NAME_ONSETS));//11
        
        oscSender.sendMessage(m, false);
        
        
    }
    
    //timeline data
    std::map<string, float> timelineValues = timePanel.getTracksValues();
    
    for (auto& kv : timelineValues){
        //cout<<"timeline send osc :: "<<kv.first<<" -- "<<kv.second<<endl;
        
        string key = kv.first;
        float floatValue = kv.second;
        
        ofxOscMessage m;
        m.setAddress("/" + key);//address: "/TL-(trackName)"
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
    //???
   /// mainPanel.loadSettings(_projectDir);
    timePanel.loadSettings(_projectDir);
   ///  metersPanel.loadSettings(_projectDir);
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
    
    dataSaver.reset();
    
    if(_projectDir=="")
        ofLogVerbose() << "ofApp: Settings LOADED from data/";
    else
        ofLogVerbose() << "ofApp: Settings LOADED from: "<<_projectDir;
}
//--------------------------------------------------------------
#pragma mark - Save Analysis Data

//-------------------------------------------------------------
void ofApp::drawSavingAnalysisSign(){
    ofPushStyle();
    
    ofFill();
    ofSetColor(0,150);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255);
    
    string displayStr = "Saving Analysis Data...  " + ofToString(dataSaver.getPercentage(), 2) + "%";
    
    //align center
    int label_w = verdana.stringWidth(displayStr);
    int label_x =  ofGetWidth() * .5 - label_w *.5;
    
    verdana.drawString(displayStr, label_x , ofGetHeight()/2);
    
    
    ofPopStyle();
}

//------------------------------------------------------------
#pragma mark - OF core other tools
//------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
    ofLogVerbose()<<"**** Window resized: "<< w <<"x"<< h;
    
    int mainH = MAIN_PANEL_HEIGHT * h;
    int timeH = TIME_PANEL_HEIGHT * h;
    int metersH = METER_PANEL_HEIGHT * h;
    
    mainPanel.resize(w, mainH);
    metersPanel.resize(mainH+timeH, w, metersH);
    timePanel.resize(mainH, w, timeH);
    
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //for testing...
    //cout<<"Mouse Released: "<<x<<"-"<<y<<"-"<<button<<endl;
    
    timePanel.checkIfHeightChanged();
    timePanel.checkIfWaveformPreviewChanged();
}
//--------------------------------------------------------------
#pragma mark - OF Not USed
//-----------------------------------------------------------
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
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
