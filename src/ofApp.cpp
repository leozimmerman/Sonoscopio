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

#include "ofApp.h"

#pragma mark - Core funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    setupOFContext();
    setupTimeMeasurment();
    setupPanels();
    setupListeners();

    oscSender.setup(config.osc().host,  config.osc().port);
    setupModals();
    dataSaver.setup(ofGetAppPtr());
    
    verdana.load("gui_assets/fonts/verdana.ttf", 25, false, false);
}

void ofApp::setupOFContext() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetBackgroundColor(ofColor::black);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    config.setFrameRate(INIT_FPS);
    ofSetFrameRate(INIT_FPS);
}

void ofApp::setupTimeMeasurment() {
    
    TIME_SAMPLE_SET_FRAMERATE(config.getFrameRate()); //set the app's target framerate (MANDATORY)
    //specify where the widget is to be drawn
    TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );
    TIME_SAMPLE_SET_AVERAGE_RATE(0.1);    //averaging samples, (0..1],
    //1.0 gets you no averaging at all
    //use lower values to get steadier readings
    TIME_SAMPLE_DISABLE_AVERAGE();    //disable averaging
    TIME_SAMPLE_SET_REMOVE_EXPIRED_THREADS(true); //inactive threads will be dropped from the table
    TIME_SAMPLE_DISABLE();
}

void ofApp::setupPanels() {
    int w = ofGetWidth();
    int h = ofGetHeight();
    updatePanelsDimensions(w, h);
    
    mainPanel.setup(0, 0, w, _main_height);
    metersPanel.setup(0, mainPanel.maxY(), _meters_width, (h - mainPanel.maxY()));
    timePanel.setup(metersPanel.maxX(), mainPanel.maxY(), (w - metersPanel.maxX()), (h - mainPanel.maxY()), INIT_AUDIO_FILE);
   
    mainPanel.setFrameRate(MAIN_PANEL_FPS);
    timePanel.setFrameRate(TL_PANEL_FPS);
    metersPanel.setFrameRate(MT_PANEL_FPS);
    
    config.setAnalysisMode(INIT_ANALYSIS_MODE);
    config.setBufferSize(INIT_BUFFER_SIZE);
    config.setProjectDir(INIT_PROJECT_DIR);
    config.setOscConfiguration(INIT_OSC_HOST, INIT_OSC_PORT,  TRUE, TRUE);
    
    if (timePanel.isFileLoaded()) {
        mainPanel.setFileInfoString(timePanel.getFileInfo());
        config.setChannelsNum(timePanel.getNumChannels());
        config.setSampleRate(timePanel.getSampleRate());
        mainAnalyzer.setup(config.getSampleRate(), config.getBufferSize(), 1);
        metersPanel.setChannelAnalyzers(mainAnalyzer.getChannelAnalyzersPtrs());//FIXME. separar el setup de los analyzers!
    }
}

void ofApp::setupModals() {
    mText = make_shared<TextModal>();
    mText->addListener(this, &ofApp::onModalEvent);
    mMenu = make_shared<MenuModal>();
    mMenu->addListener(this, &ofApp::onModalEvent);
    mMenu->setMainAppPtr(ofGetAppPtr());
}

void ofApp::setupListeners() {
    ofAddListener(OnsetMeterView::onsetEventGlobal, this, &ofApp::onsetFired);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(dataSaver.isThreadRunning()){
        return;
    }

    ofSetWindowTitle("Sonoscopio");//("Sonoscopio - " + ofToString(ofGetFrameRate(),2));
    
    //analyze soundBuffer----------------
    if (timePanel.isFileLoaded()){
        ofSoundUpdate();
        
        audioMutex.lock();
        
        TS_START("GET-AUDIO-BUFFERS");
        if(config.getAnalysisMode()==SPLIT){
            soundBuffer = timePanel.getCurrentSoundBuffer(config.getBufferSize());//multichannel soundbuffer
        }else if(config.getAnalysisMode()==MONO){
            soundBuffer = timePanel.getCurrentSoundBufferMono(config.getBufferSize());//mono soundbuffer
        }
        TS_STOP("GET-AUDIO-BUFFERS");
        
        
        TS_START("AUDIO-ANALYSIS");
        if(timePanel.isPlaying()){
            mainAnalyzer.analyze(soundBuffer);
        }
        TS_STOP("AUDIO-ANALYSIS");
        
        audioMutex.unlock();
    }

    //update panels-------------------
    TS_START("PANELS-UPDATE");
    mainPanel.update();
    timePanel.update();
    metersPanel.update();
    TS_STOP("PANELS-UPDATE");
    
    //send OSC-----------------------
    TS_START("SEND-OSC");
    if(config.osc().bSend) {
    //FIXME: No anda
        ///oscSender.sendOscData(metersPanel.metersView.getMetersValues(), metersPanel.metersView.getMetersVectorValues(), timePanel.timelineView.getTracksValues(), config.osc().bSendVectorValues);
    }
    TS_STOP("SEND-OSC");


}

//--------------------------------------------------------------
void ofApp::draw(){
 
    //If saving thread is running dont draw Panels.
    if(dataSaver.isThreadRunning()){
        drawSavingAnalysisSign();
        return;
    }
    
    TS_START("TIMELINE-PANEL");
    timePanel.draw();
    TS_STOP("TIMELINE-PANEL");
    
    TS_START("METERS-PANEL");
    metersPanel.draw();
    TS_STOP("METERS-PANEL");
    
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
    if(mainPanel.getFocused() || timePanel.getFocused() || metersPanel.getFocused() || mMenu->getFocused()){
        return;
    }
    //---------------------------
    /*
     * 'e' expands focused track
     * 'd' enables/disables focused track
     * 'a' adjust tracks height shorcut
     */
    timePanel.keyPressed(key);
    //---------------------------
    /* TODO: REMOVE
     * '1' scrollUp
     * '2' scrollDown
     */
    metersPanel.keyPressed(key);
    
    //--------------------------------
    /*
     * 't': time measurement on/off
     * 'm': add marker
     * 'w': rewind
     * 'k': add keyframe in focused track
     */
    switch (key) {
        
        case 'w':
            rewind();
            break;
            
        case 'k':
            timePanel.addKeyframeInFocusedTrack();
            break;
            
        case 'q':
            showKeyboardShortcuts();
            break;
            
        case 'z':
            showMenu();
            break;
            
        case 't':
            if(TIME_SAMPLE_GET_ENABLED()) TIME_SAMPLE_DISABLE();
            else TIME_SAMPLE_ENABLE();
            break;
            
        default:
            break;
    }
    
    if (ofGetModifierShortcutKeyPressed()) {
        switch (key) {
            case '1':
                //setViewMode(ALL);
                break;
            case '2':
                //setViewMode(TIMEPANEL_ONLY);
                break;
            case '3':
                //setViewMode(METERSPANEL_ONLY);
                break;
                
            case 'm':
                //timePanel.addMarker();
                break;
                
            case 't':
                if(TIME_SAMPLE_GET_ENABLED()) TIME_SAMPLE_DISABLE();
                else TIME_SAMPLE_ENABLE();
                break;
        }
        

    }
    
    
    
}
#pragma mark - Audio Engine funcs
//--------------------------------------------------------------
void ofApp::openAudioFile(string filename){
    audioMutex.lock();
    timePanel.openAudioFile(filename);
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    config.setChannelsNum(timePanel.getNumChannels());
    config.setSampleRate(timePanel.getSampleRate());
    resetAnalysisEngine();
    dataSaver.reset();
    audioMutex.unlock();
}
//--------------------------------------------------------------
void ofApp::setAnalysisMode(AnalysisMode mode){
    
    stop();
    
    audioMutex.lock();
    config.setAnalysisMode(mode);
    resetAnalysisEngine();
    
    audioMutex.unlock();
}
//--------------------------------------------------------------
void ofApp::setBufferSize(int bs){
    
    stop();
    audioMutex.lock();
    config.setBufferSize(bs);
    resetAnalysisEngine();
    audioMutex.unlock();
}
//--------------------------------------------------------------
void ofApp::resetAnalysisEngine(){
    
    int this_channelNum;
    if(config.getAnalysisMode() == SPLIT){
        this_channelNum = config.getChannelsNum();
    }else if(config.getAnalysisMode() == MONO){
        this_channelNum = 1;
    }

    mainAnalyzer.reset(config.getSampleRate(), config.getBufferSize(), this_channelNum);
    metersPanel.reset(mainAnalyzer.getChannelAnalyzersPtrs());
    dataSaver.reset();
}

//--------------------------------------------------------------
void ofApp::setFrameRate(int fps){
    
    stop();
    
    config.setFrameRate(fps);
    ofSetFrameRate(fps);
    timePanel.setFrameRate(fps);
    
    //update file info frames num info:
    mainPanel.setFileInfoString(timePanel.getFileInfo());
    
    dataSaver.updateFrameRate();
    
    TIME_SAMPLE_SET_FRAMERATE(fps);
    
    ofLogVerbose()<<"Frame Rate changed to: "<<ofToString(fps);
}
//--------------------------------------------------------------
#pragma mark - Playback Controls
//--------------------------------------------------------------
void ofApp::togglePlay(){
    timePanel.togglePlay();
}
//--------------------------------------------------------------
void ofApp::stop(){
    timePanel.stop();
}
//--------------------------------------------------------------
void ofApp::rewind(){
    timePanel.rewind();
}
//--------------------------------------------------------------
#pragma mark - Settings funcs
//--------------------------------------------------------------
void ofApp::openProject(string projectDir ){
   
    stop();
    
    ofLogVerbose()<<"ofApp Opening project in :"<<projectDir<<endl;
    
    std::string projDir = projectDir + "/";
    
    //-----------------------------
    //-:Check if project dir is correct
    ofDirectory dir(projDir);
    
    //-----------------------------
    //-:Check settings folders
    if(!dir.doesDirectoryExist(projDir + "main_settings")){
        ofLogError()<< "ofApp openProject: No main_settings folder found in the project directory.";
        return;
    }else if(!dir.doesDirectoryExist(projDir + "meters_settings")){
        ofLogError()<< "ofApp openProject: No meters_settings folder found in the project directory.";
        return;
    }else if(!dir.doesDirectoryExist(projDir + "timeline_settings")){
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
    
    if(dir.getPath(0) == projDir+"audiofile.wav" || dir.getPath(0) == projDir+"audiofile.mp3"){
        audioFileName = dir.getPath(0);//set audiofile name
    }else{
        ofLogError()<< "ofApp openProject: Audio file name is incorrect. Must be named audiofile.wav or audiofile.mp3";
        return;
    }
    
    //-----------------------------
    //-:Load audiofile and settings
    
    openAudioFile(audioFileName);
   
    mainPanel.loadSettings(projDir);
    timePanel.loadSettings(projDir);
    metersPanel.loadSettings(projDir);
    config.setProjectDir(projDir);
}
//--------------------------------------------------------------
//??? Add closeProject?
//--------------------------------------------------------------
void ofApp::saveSettings(){
    std::string dir = config.getProjectDir();
    mainPanel.saveSettings(dir);
    timePanel.saveSettings(dir);
    metersPanel.saveSettings(dir);
    
    if(dir=="")
        ofLogVerbose() << "ofApp: Settings SAVED to data/";
    else
        ofLogVerbose() << "ofApp: Settings SAVED to: "<<dir;
}
//--------------------------------------------------------------
void ofApp::loadSettings(){
    std::string dir = config.getProjectDir();
    stop();
    
    mainPanel.loadSettings(dir);
    timePanel.loadSettings(dir);
    metersPanel.loadSettings(dir);
    
    dataSaver.reset();
    
    if(dir=="")
        ofLogVerbose() << "ofApp: Settings LOADED from data/";
    else
        ofLogVerbose() << "ofApp: Settings LOADED from: "<<dir;
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
//TODO: borrar
void ofApp::onTimelinePanelResize(int &h){
    int new_y = mainPanel.getHeight() + h;
    int new_h = ofGetHeight() - new_y;
    
    ofLogVerbose()<<"-- timelinePanel resized: "<< h;
}//------------------------------------------------------------
void ofApp::updatePanelsDimensions(int w, int h) {
    _main_height   = MAIN_PANEL_HEIGHT * h;
    _meters_width = METER_PANEL_WIDTH * w;
    
}
//------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //TODO: Disable dragging.
    updatePanelsDimensions(w, h);
    mainPanel.resize(mainPanel.getPosition().x,
                     mainPanel.getPosition().y,
                     w,
                     _main_height);
    metersPanel.resize(metersPanel.getPosition().x,
                       mainPanel.maxY(),
                       _meters_width,
                       (h - mainPanel.maxY()));
    timePanel.resize(metersPanel.maxX(),
                     mainPanel.maxY(),
                     (w - metersPanel.maxX()),
                     (h - mainPanel.maxY()));
    
    ofLogVerbose()<<"-- Window resized: "<< w <<"x"<< h;
    ofLogVerbose()<<"-- main height: "<< _main_height <<"- _meters_width: "<< h;
}
//--------------------------------------------------------------
#pragma mark - Other
//--------------------------------------------------------------
void ofApp::onsetFired(int & panelId){
    timePanel.addKeyframeInFocusedTrack();
}
//--------------------------------------------------------------
void ofApp::onModalEvent(ofxModalEvent e) {
    if (e.type == ofxModalEvent::SHOWN){
        // dispatched when the window has finished animating in //
    }    else if (e.type == ofxModalEvent::HIDDEN){
        // dispatched when the window has finished animating out //
    }    else if (e.type == ofxModalEvent::CONFIRM){
        // dispatched when the button at index 0 is selected //
        cout << "OK button was selected" << endl;
    }    else if (e.type == ofxModalEvent::CANCEL){
        // dispatched when the button at index 1 is selected //
        cout << "CANCEL button was selected" << endl;
    }
    
   
}
//--------------------------------------------------------------
void ofApp::showKeyboardShortcuts(){
    
    string title = "KEYBOARD SHORTCUTS";
    string msg = "'cmd 1,2,3: view modes | 'cmd + t': time measurement on/off  |  'cmd + m': add marker  |  'w': rewind  |  'e': expands focused track  |  'd': enables/disables focused track  |  'a': adjust tracks height shortcut";
    
    mText->display(title, msg);
}
//--------------------------------------------------------------
void ofApp::showMenu(){
    mMenu->display(ofGetHeight());
}

#pragma mark - OF Native
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //for testing...
    //cout<<"Mouse Released: "<<x<<"-"<<y<<"-"<<button<<endl;
    
    //timePanel.checkIfHeightChanged();
    //timePanel.checkIfWaveformPreviewChanged();
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
