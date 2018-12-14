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
#include "AnalysisDataSaver.h"
#include "FileManager.h"

ofEvent<string> ofApp::errorEvent = ofEvent<string>();

#pragma mark - Core funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    setupOFContext();
    setupTimeMeasurment();
    setupPanels();
    setupListeners();
    setupModals();

    setFrameRate(INIT_FPS);
    
}

void ofApp::setupOFContext() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetBackgroundColor(ofColor::black);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    //ofSetFrameRate(INIT_FPS);
}

void ofApp::setupTimeMeasurment() {
    
    TIME_SAMPLE_SET_FRAMERATE(INIT_FPS); //set the app's target framerate (MANDATORY)
    //specify where the widget is to be drawn
    TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_BOTTOM_LEFT );
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
    timePanel.setup(metersPanel.maxX(), mainPanel.maxY(), (w - metersPanel.maxX()), (h - mainPanel.maxY()));
   
    metersPanel.setupAnalyzer(INIT_SAMPLE_RATE, INIT_BUFFER_SIZE, 1);
    
    mainPanel.setFrameRate(MAIN_PANEL_FPS);
    timePanel.setFrameRate(TL_PANEL_FPS);
    metersPanel.setFrameRate(MT_PANEL_FPS);
}

void ofApp::setupModals() {
    queuedErrorMessage = "";
    mText = make_shared<TextModal>();
    mText->addListener(this, &ofApp::onModalEvent);
}

void ofApp::setupListeners() {
    
    ofAddListener(ofApp::errorEvent, this, &ofApp::errorSent);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(AnalysisDataSaver::getInstance().isThreadRunning()){
        return;
    }
    string windowTitle = "Sonoscopio";
    if (FileManager::getInstance().isFileLoaded()){
        windowTitle += " - " + FileManager::getInstance().getBaseName();
    }
    ofSetWindowTitle(windowTitle);//("Sonoscopio - " + ofToString(ofGetFrameRate(),2));
    
    if (timePanel.isFileLoaded() && timePanel.isPlaying()){
        ofSoundUpdate();
        ofSoundBuffer soundBuffer = timePanel.getCurrentSoundBufferMono(metersPanel.getBufferSize());
        TS_START("AUDIO-ANALYSIS");
        metersPanel.analyzeBuffer(soundBuffer);
        TS_STOP("AUDIO-ANALYSIS");
    }

    TS_START("PANELS-UPDATE");
    mainPanel.update();
    timePanel.update();
    metersPanel.update();
    TS_STOP("PANELS-UPDATE");
}


void ofApp::draw(){

    if(AnalysisDataSaver::getInstance().drawSavingAnalysisSign()){
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

void ofApp::exit(){
    metersPanel.exit();
    AnalysisDataSaver::getInstance().stop();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(mainPanel.getFocused() || timePanel.getFocused() || metersPanel.getFocused()){
        return;
    }
    
    if (mainPanel.keyPressed(key)){
        return;
    }
    //TODO: Edit actions should add 'Cmd'.
    /* TimelinePanel:
     * 'e' expands focused track
     * 'd' enables/disables focused track
     * 'a' adjust tracks height shorcut
     * 'm': add marker
     * 'w': rewind
     * 'k': add keyframe in focused track
     */
    else if (timePanel.keyPressed(key)){
        return;
    }
    //---------------------------
    /* TODO: REMOVE
     * '1' scrollUp
     * '2' scrollDown
     */
    else if (metersPanel.keyPressed(key)){
        return;
    }
    
    //--------------------------------
    /*
     * 't': time measurement on/off
     * 'q': keyboardshortcuts
     */
    switch (key) {
        case 'q':
            showKeyboardShortcuts();
            break;
        case 't':
            toggleTimeMeasurement();
            break;
        default:
            break;
    }
    
    //'Cmd' modifier
    if (ofGetModifierShortcutKeyPressed()) {
        switch (key) {
            case '1':
                break;
            case 'm':
                
                break;

        }

    }
    
}

void ofApp::setFrameRate(int fps){
    
    ofSetFrameRate(fps);
    timePanel.setFrameRate(fps);
    
    //TODO: update file info frames num info:
    //-> mainPanel.updateFileInfoString(fps, framesNum;
    
    AnalysisDataSaver::getInstance().updateFrameRate(fps, timePanel.getTotalFramesNum());
    
    TIME_SAMPLE_SET_FRAMERATE(fps);
}

#pragma mark - Sizes
void ofApp::updatePanelsDimensions(int w, int h) {
    _main_height   = GUI_COMP_HEIGHT + FILE_INFO_HEIGHT;
    _meters_width = METER_PANEL_WIDTH * w;
}

void ofApp::windowResized(int w, int h){
    //TODO: Disable dragging ??
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
    
    ofLogVerbose()<<"ofApp: Window resized: "<< w <<"x"<< h;
}

#pragma mark - Other

void ofApp::errorSent(string & errorMessage){
    showErrorMessage(errorMessage);
}

void ofApp::onModalEvent(ofxModalEvent e) {
    if (e.type == ofxModalEvent::SHOWN){
        // dispatched when the window has finished animating in //
    }    else if (e.type == ofxModalEvent::HIDDEN){
        // dispatched when the window has finished animating out //
        if (queuedErrorMessage != ""){
            showErrorMessage(queuedErrorMessage);
            queuedErrorMessage = "";
        }
    }    else if (e.type == ofxModalEvent::CONFIRM){
        // dispatched when the button at index 0 is selected //
        cout << "ofApp: OK button was selected" << endl;
    }    else if (e.type == ofxModalEvent::CANCEL){
        // dispatched when the button at index 1 is selected //
        cout << "ofApp: CANCEL button was selected" << endl;
    }
    
   
}

void ofApp::showKeyboardShortcuts(){
    string title = "KEYBOARD SHORTCUTS";
    string msg = KEYBOARD_SHORTCUTS_MSG;
    mText->display(title, msg);
}

void ofApp::showErrorMessage(string message){
    string title = "ERROR";
    if (mText->visible()) {
        queuedErrorMessage = message;
        return;
    }
    mText->display(title, message);
}

void ofApp::toggleTimeMeasurement(){
    if(TIME_SAMPLE_GET_ENABLED()) TIME_SAMPLE_DISABLE();
    else TIME_SAMPLE_ENABLE();
}
