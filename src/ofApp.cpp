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

void ofApp::setup(){
    
    setupOFContext();
    setupTimeMeasurment();
    setupPanels();
    setupListeners();
    setupModals();

    setFrameRate(INIT_FPS);
}

void ofApp::update(){
    
    if(AnalysisDataSaver::getInstance().isThreadRunning()){
        return;
    }
    string windowTitle = "Sonoscopio";
    if (FileManager::getInstance().isFileLoaded()){
        windowTitle += " - " + FileManager::getInstance().getBaseName();
    }
    if (ofGetFrameRate() < (mainPanel.getSettingsFrameRate() - 10)) {
        windowTitle += " | " + ofToString(ofGetFrameRate(),2);
    }
    ofSetWindowTitle(windowTitle);
    
    if (timelinePanel.isFileLoaded() && timelinePanel.isPlaying()){
        ofSoundUpdate();
        ofSoundBuffer soundBuffer = timelinePanel.getCurrentSoundBufferMono(metersPanel.getBufferSize());
        TS_START("AUDIO-ANALYSIS");
        metersPanel.analyzeBuffer(soundBuffer);
        TS_STOP("AUDIO-ANALYSIS");
    }

    TS_START("PANELS-UPDATE");
    mainPanel.update();
    timelinePanel.update();
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
    timelinePanel.draw();
    TS_STOP("TIMELINE-PANEL");
    
    TS_START("MAIN-PANEL");
    mainPanel.draw();
    TS_STOP("MAIN-PANEL");
}

void ofApp::exit(){
    metersPanel.exit();
    AnalysisDataSaver::getInstance().stop();
}

void ofApp::keyPressed(int key){
    
    if(mainPanel.getFocused() || timelinePanel.getFocused() || metersPanel.getFocused()){
        return;
    }
    
    /* Meters Panel
     * 'cmd+o': Open File
     * 'cmd+s': Save Settings
     */
    if (mainPanel.keyPressed(key)){
        return;
    }
    
    /* TimelinePanel:
     * 'e' expands focused track
     * 'd' enables/disables focused track
     * 'a' adjust tracks height shorcut
     * 'm': add marker
     * 'w': rewind
     * 'k': add keyframe in focused track
     */
    else if (timelinePanel.keyPressed(key)){
        return;
    }
    
    /* Meters Panel
     * 'ARROW-UP': scrollUp
     * 'ARROW-DOWN': scrollDown
     */
    else if (metersPanel.keyPressed(key)){
        return;
    }
    
    //'Cmd' modifier
    //* 'cmd+t': time measurement on/off
    //* 'cmd+y': keyboardshortcuts
    if (ofGetModifierShortcutKeyPressed()) {
        switch (key) {
            case 't':
                toggleTimeMeasurement();
                break;
            case 'y':
                //TODO: Depreacte this functionality:
                showKeyboardShortcuts();
                break;
        }
    }
    
}

#pragma mark - Setups

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
    timelinePanel.setup(metersPanel.maxX(), mainPanel.maxY(), (w - metersPanel.maxX()), (h - mainPanel.maxY()));
        
    mainPanel.setFrameRate(MAIN_PANEL_FPS);
    timelinePanel.setFrameRate(TL_PANEL_FPS);
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

void ofApp::setFrameRate(int fps){
    
    ofSetFrameRate(fps);
    timelinePanel.setFrameRate(fps);
    
    //TODO: update file info frames num info:
    //-> mainPanel.updateFileInfoString(fps, framesNum;
    
    AnalysisDataSaver::getInstance().updateFrameRate(fps, timelinePanel.getTotalFramesNum());
    
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
    timelinePanel.resize(metersPanel.maxX(),
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
        setPanelsClicksEnabled(false);
    } else if (e.type == ofxModalEvent::HIDDEN){
        // dispatched when the window has finished animating out //
        if (queuedErrorMessage != ""){
            showErrorMessage(queuedErrorMessage);
            queuedErrorMessage = "";
        }
        setPanelsClicksEnabled(true);
    } else if (e.type == ofxModalEvent::CONFIRM){
        // dispatched when the button at index 0 is selected //
        cout << "ofApp: OK button was selected" << endl;
    } else if (e.type == ofxModalEvent::CANCEL){
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

void ofApp::setPanelsClicksEnabled(bool enabled){
    mainPanel.setClicksEnabled(enabled);
    timelinePanel.setClicksEnabled(enabled);
    metersPanel.setClicksEnabled(enabled);
}
