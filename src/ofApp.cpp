/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * Sonoscopio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

//**********************************************************************

//TODO: Check Time Measurements of Drawing funcs. Tarda mucho. En 60 fps, no se banca muchos canales.

//**********************************************************************

#include "ofApp.h"

#pragma mark - Core funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    //Gral---------------------------
    ofSetBackgroundColor(ofColor::black);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    _frameRate = INIT_FPS;
    ofSetFrameRate(_frameRate);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //Time Measurement-----------------
    
    TIME_SAMPLE_SET_FRAMERATE( _frameRate ); //set the app's target framerate (MANDATORY)
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
    _timePanelHeightPercent   = TIME_PANEL_HEIGHT;
    _metersPanelHeightPercent = METER_PANEL_HEIGHT;
    
    mainPanel.setup(0, 0, ofGetWidth(), mainH, ofGetAppPtr());
    timePanel.setup(0, mainH, ofGetWidth(), timeH, ofGetAppPtr());
   
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    ofAddListener(timePanel.heightResizedEvent, this, &ofApp::onTimelinePanelResize);
    ofAddListener(ofxAAOnsetMeter::onsetEventGlobal, this, &ofApp::onsetFired);
    
    //Audio info--------------
    _channelsNum = timePanel.audioTrack->getNumChannels();
    _samplerate = timePanel.audioTrack->getSampleRate();
    //_totalFramesNum = timePanel.timeline.getDurationInFrames();
    
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
    _bSendOscVectorValues = TRUE;
    
    //MODAL-------------------------------
    mAlert = make_shared<ofxModalAlert>();
    mAlert->addListener(this, &ofApp::onModalEvent);
    
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
    
    
    
    //if dataSaver thread is running do not update anything and wait for it to finish
    if(dataSaver.isThreadRunning()){
        return;
    }
    
    //**********************************************************************
    
    ofSetWindowTitle("Sonoscopio - " + ofToString(ofGetFrameRate(),2));
    
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
 
    //If saving thread is running dont draw Panels.
    if(dataSaver.isThreadRunning()){
        drawSavingAnalysisSign();
        return;
    }
    
    TS_START("METERS-PANEL");
    metersPanel.draw();
    TS_STOP("METERS-PANEL");

    TS_START("TIMELINE-PANEL");
    timePanel.draw();
    TS_STOP("TIMELINE-PANEL");
    
    TS_START("MAIN-PANEL");
    mainPanel.draw();
    TS_STOP("MAIN-PANEL");
    
 
    
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
     * 't': time measurement on/off
     * 'm': add marker
     * 'w': rewind
     * 'k': add keyframe in focused track
     */
    switch (key) {
        
        case 't':
            if(TIME_SAMPLE_GET_ENABLED()) TIME_SAMPLE_DISABLE();
            else TIME_SAMPLE_ENABLE();
            break;
    
        case 'm':
            timePanel.addMarker();
            break;
        
        case 'w':
            rewind();
            break;
            
        case 'k':
            addKeyframeInFocusedTrack();
            break;
            
        //TODO: sacar de aca, poner en main panel
  
        case '1':
            setViewMode(ALL);
            break;
        case '2':
            setViewMode(TIMEPANEL_ONLY);
            break;
        case '3':
            setViewMode(METERSPANEL_ONLY);
            break;
            
        case 'q':
            mAlert->alert("It's time to go outside.");
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
    //_totalFramesNum = timePanel.timeline.getDurationInFrames();
    
    resetAnalysisEngine();
    
    dataSaver.reset();

    audioMutex.unlock();
    
    
    
}


//--------------------------------------------------------------
void ofApp::setAnalysisMode(analysisMode mode){
    
    stop();
    
    audioMutex.lock();
    
    _currentAnalysisMode = mode;
    resetAnalysisEngine();
    
    audioMutex.unlock();
}
//--------------------------------------------------------------
void ofApp::setBufferSize(int bs){
    
    stop();
    
    audioMutex.lock();
    
    _bufferSize = bs;
    resetAnalysisEngine();
    
    audioMutex.unlock();
    
}
//--------------------------------------------------------------
void ofApp::setFrameRate(int fps){
    
    stop();
    
    _frameRate = fps;
    ofSetFrameRate(_frameRate);
    timePanel.setFrameRate(_frameRate);
    
    //update file info frames num info:
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    
    dataSaver.updateFrameRate();
    
 
    TIME_SAMPLE_SET_FRAMERATE( _frameRate );
    
    ofLogVerbose()<<"Frame Rate changed to: "<<ofToString(fps);
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
    dataSaver.reset();
}
//--------------------------------------------------------------
#pragma mark - Playback Controls
//--------------------------------------------------------------
void ofApp::togglePlay(){
    timePanel.timeline.togglePlay();
}
//--------------------------------------------------------------
void ofApp::stop(){
    timePanel.timeline.stop();
}
//--------------------------------------------------------------
void ofApp::rewind(){
    timePanel.timeline.setCurrentTimeToInPoint();
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
    
    
    //-------------------------------------------------
    //-:Send Meters Values
    
    vector<std::map<string, float>> metersValues = metersPanel.getMetersValues();
    vector<std::map<string, vector<float>>> metersVectorValues = metersPanel.getMetersVectorValues();
    
    if(metersVectorValues.size() != metersValues.size()){
        ofLogError()<<"ofApp sendOscData: metersValues and metersVectorValues not matching.";
        return;
    }
   
    

    for(int i=0; i<metersValues.size(); i++){
        
        //"i" -> channel
        
        //Send Single Values-----------------
        ofxOscMessage msg;
        
        //address: "/ch0" - "/ch1" - "/ch2" etc...
        string address = "/ch" + ofToString(i);
        msg.setAddress(address);
        
        //->sames order as Osc Indexes (ofxAudioAnalyzerAlgorithms.h)
        msg.addFloatArg(metersValues[i].at(MTR_NAME_POWER));//0
        msg.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_FREQ));//1
        msg.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_CONF));//2
        msg.addFloatArg(metersValues[i].at(MTR_NAME_PITCH_SALIENCE));//3
        msg.addFloatArg(metersValues[i].at(MTR_NAME_HFC));//4
        msg.addFloatArg(metersValues[i].at(MTR_NAME_CENTROID));//5
        msg.addFloatArg(metersValues[i].at(MTR_NAME_SPEC_COMP));//6
        msg.addFloatArg(metersValues[i].at(MTR_NAME_INHARMONICTY));//7
        msg.addFloatArg(metersValues[i].at(MTR_NAME_DISSONANCE));//8
        msg.addFloatArg(metersValues[i].at(MTR_NAME_ROLL_OFF));//9
        msg.addFloatArg(metersValues[i].at(MTR_NAME_ODD_TO_EVEN));//10
        msg.addInt32Arg(metersValues[i].at(MTR_NAME_ONSETS));//11
        
        oscSender.sendMessage(msg, false);//???ADD OSC BUNDLES?
        
        if(_bSendOscVectorValues){
            
            //Send MelBands-------------------------
            
            ofxOscMessage msgMel;
            
            //address: "/ch0mel" - "/ch1mel" - "/ch2mel" etc...
            address = "/ch" + ofToString(i) + "mel";
            msgMel.setAddress(address);
            
            //cout<<"melbands size: "<< metersVectorValues[i].at(MTR_NAME_MEL_BANDS).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_MEL_BANDS).size(); j++){
                msgMel.addFloatArg(metersVectorValues[i].at(MTR_NAME_MEL_BANDS)[j]);
            }
            oscSender.sendMessage(msgMel, false);
            
            //Send MFCC-----------------------------
            
            ofxOscMessage msgMfcc;
            
            //address: "/ch0mfcc" - "/ch1mfcc" - "/ch2mfcc" etc...
            address = "/ch" + ofToString(i) + "mfcc";
            msgMfcc.setAddress(address);
            
            //cout<<"mfcc size: "<< metersVectorValues[i].at(MTR_NAME_MFCC).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_MFCC).size(); j++){
                msgMfcc.addFloatArg(metersVectorValues[i].at(MTR_NAME_MFCC)[j]);
            }
            oscSender.sendMessage(msgMfcc, false);
            
            //Send MFCC-----------------------------
            
            ofxOscMessage msgHpcp;
            
            //address: "/ch0hpcp" - "/ch1hpcp" - "/ch2hpcp" etc...
            address = "/ch" + ofToString(i) + "hpcp";
            msgHpcp.setAddress(address);
            
            //cout<<"hpcp size: "<< metersVectorValues[i].at(MTR_NAME_HPCP).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_HPCP).size(); j++){
                msgHpcp.addFloatArg(metersVectorValues[i].at(MTR_NAME_HPCP)[j]);
            }
            oscSender.sendMessage(msgHpcp, false);
            
            
            //Send Tristimulus-----------------------------
            
            ofxOscMessage msgTris;
            
            //address: "/ch0tris" - "/ch1tris" - "/ch2tris" etc...
            address = "/ch" + ofToString(i) + "tris";
            msgTris.setAddress(address);
            
            //cout<<"tris size: "<< metersVectorValues[i].at(MTR_NAME_TRISTIMULUS).size()<<" add: "<< address << endl;
            
            for (int j=0; j<metersVectorValues[i].at(MTR_NAME_TRISTIMULUS).size(); j++){
                msgTris.addFloatArg(metersVectorValues[i].at(MTR_NAME_TRISTIMULUS)[j]);
            }
            oscSender.sendMessage(msgTris, false);

        
        }
        
    }
    
    //-------------------------------------------------
    //-:Send Timeline Tracks Values (one msg x each track)
    std::map<string, float> timelineValues = timePanel.getTracksValues();
    for (auto& kv : timelineValues){
        //cout<<"timeline send osc :: "<<kv.first<<" -- "<<kv.second<<endl;
        
        string key = kv.first;
        float floatValue = kv.second;
        
        ofxOscMessage msg;
        msg.setAddress("/" + key);//address: "/TL-(trackName)"
        msg.addFloatArg(floatValue);
        oscSender.sendMessage(msg, false);
        
    }

}
//--------------------------------------------------------------
#pragma mark - Settings funcs
//--------------------------------------------------------------
void ofApp::openProject(string projectDir ){
   
    stop();
    
    ofLogVerbose()<<"ofApp Opening project in :"<<projectDir<<endl;
    
    _projectDir = projectDir + "/";
    
    //-----------------------------
    //-:Check if project dir is correct
    ofDirectory dir(_projectDir);
    
    //-----------------------------
    //-:Check settings folders
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
    
    //-----------------------------
    //-:Check audiofile:
    dir.allowExt("wav");
    dir.allowExt("mp3");
    //populate the directoryr
    dir.listDir();
    
    //-----------------------------
    //-:Check there is AT LEAST and ONLY one file
    if(dir.size() == 0){
        ofLogError()<< "ofApp openProject: No audio file found in the project directory. Only wav and mp3 extensions.";
        return;
    }else if(dir.size()>1){
        ofLogError()<< "ofApp openProject: More than one audio file found in project directory. Must be only one.";
        return;
    }
    
    //-----------------------------
    //-:Check audio file name
    string audioFileName;
    
    if(dir.getPath(0) == _projectDir+"audiofile.wav" || dir.getPath(0) == _projectDir+"audiofile.mp3"){
        audioFileName = dir.getPath(0);//set audiofile name
    }else{
        ofLogError()<< "ofApp openProject: Audio file name is incorrect. Must be named audiofile.wav or audiofile.mp3";
        return;
    }
    
    //-----------------------------
    //-:Load audiofile and settings
    
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
    
    stop();
    
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
//??? Add dataSaver.stop() func?
void ofApp::saveAnalysisDataToFile(){
    
    stop();
    
    dataSaver.start();
}
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
#pragma mark - Sizes
//--------------------------------------------------------------
void ofApp::onTimelinePanelResize(int &h){
    
    int new_y = mainPanel.getHeight() + h;
    int new_h = ofGetHeight() - new_y;
    
    metersPanel.adjustPosAndHeight(new_y, new_h);
    
    ofLogVerbose()<<"-- timelinePanel resized: "<< h;
}
//------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
    
    int mainH   = MAIN_PANEL_HEIGHT * h;
    int timeH   = _timePanelHeightPercent * h;
    int metersH = _metersPanelHeightPercent * h;
    
    mainPanel.resize(w, mainH);
    metersPanel.resize(mainH+timeH, w, metersH);
    timePanel.resize(mainH, w, timeH);
    
    ofLogVerbose()<<"-- Window resized: "<< w <<"x"<< h;
    
}
//--------------------------------------------------------------
//This function belongs to ofApp instead of metersPanel because panels sizes are timelinePanel dependant.
void ofApp::hideMetersPanel(bool hide){
    
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    metersPanel.setHidden(hide);
    
    if (hide){
        _timePanelHeightPercent   = 0.85;
        _metersPanelHeightPercent = 0.0;
    } else {
        _timePanelHeightPercent = TIME_PANEL_HEIGHT;
        _metersPanelHeightPercent = METER_PANEL_HEIGHT;
    }
    windowResized(w, h);
    
}
//--------------------------------------------------------------
void ofApp::setViewMode(viewMode mode){
    _currentViewMode = mode;
    
    switch (_currentViewMode) {
            
        case ALL:
            hideMetersPanel(false);
            timePanel.setHidden(false);
            break;
            
        case TIMEPANEL_ONLY:
            timePanel.setHidden(false);
            hideMetersPanel(true);
            break;
            
        case METERSPANEL_ONLY:
            hideMetersPanel(false);
            timePanel.setHidden(true);
            break;
            

        default:
            break;
    }
}
//--------------------------------------------------------------
#pragma mark - Other
//--------------------------------------------------------------
void ofApp::onsetFired(int & panelId){
    addKeyframeInFocusedTrack();
}
//--------------------------------------------------------------
void ofApp::addKeyframeInFocusedTrack(){
    timePanel.addKeyframeInFocusedTrack();
}
//--------------------------------------------------------------
void ofApp::onModalEvent(ofxModalEvent e) {
    if (e.type == ofxModalEvent::CONFIRM){
        cout << "confirm button was selected" << endl;
    }
}

#pragma mark - OF Native
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //for testing...
    //cout<<"Mouse Released: "<<x<<"-"<<y<<"-"<<button<<endl;
    
    timePanel.checkIfHeightChanged();
    timePanel.checkIfWaveformPreviewChanged();
}
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
