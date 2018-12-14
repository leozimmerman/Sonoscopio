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
#include "MainPanel.h"
#include "SettingsManager.h"
#include "FileManager.h"
#include "AnalysisDataSaver.h"
#include "OscSender.h"
#include "PanelsBridge.h"

#pragma mark - core funcs
void MainPanel::setup(int x, int y, int w, int h){
    BasePanel::setup(x, y, w, h);
    setBackgroundColor(ofColor(80));
    setupSingletons();
    
    guiView.setup(x, y, w, GUI_COMP_HEIGHT, this);
    fileInfoView.setup(x, guiView.maxY(), w, h - guiView.getHeight());
    
}

void MainPanel::setupSingletons(){
    OscSender::getInstance().setup(INIT_OSC_HOST,  INIT_OSC_PORT);
    SettingsManager::getInstance().setMainPanelPtr(this);
    FileManager::getInstance().setMainPanelPtr(this);
    PanelsBridge::getInstance().setMainPanelPtr(this);
}

void MainPanel::update(){
    guiView.update();
    
    if (guiView.getOscEnabled()) {
        OscSender::getInstance().sendOscData();
    }
}

void MainPanel::draw(){
    if (!View::mustDrawNewFrame()){
        View::drawLoadedTexture();
        return;
    }
    if (_isHidden){ return; }
    View::draw();
    
    guiView.draw();
    fileInfoView.draw();
    
    View::loadViewInTexture();
}

void MainPanel::exit(){}

bool MainPanel::keyPressed(int key){
    return false;
}

bool MainPanel::getFocused(){
    return guiView.getFocused();
}

void MainPanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, GUI_COMP_HEIGHT);
    fileInfoView.resize(x, guiView.maxY(), w, h - guiView.getHeight());
}

#pragma mark - Gui called funcs

void MainPanel::openFileDialog(){
    FileManager::getInstance().openFileDialog();
}

void MainPanel::saveAllSettings(){
    if(!FileManager::getInstance().isFileLoaded()){
        string message = "No Audio File loaded.";
        ofNotifyEvent(ofApp::errorEvent, message);
        return;
    }
    SettingsManager::getInstance().saveSettings();
}

void MainPanel::renderAnalysis(){
    if(!FileManager::getInstance().isFileLoaded()){
        string message = "No Audio File loaded.";
        ofNotifyEvent(ofApp::errorEvent, message);
        return;
    }
    AnalysisDataSaver::getInstance().start();
}

#pragma mark - Settings funcs
void MainPanel::applySettings(int fps, int buffersize, float bpm, string host, int port){
    currentSettings.frameRate = fps;
    currentSettings.osc.host = host;
    currentSettings.osc.port = port;
    
    OscSender::getInstance().setHost(host);
    OscSender::getInstance().setPort(port);
    ofApp* app = (ofApp*)ofGetAppPtr();
    app->setFrameRate(fps);
}

void MainPanel::loadSettings(MainPanelSettings& settings){
    currentSettings = settings;
    guiView.setStateFromSettings(settings);
}

void MainPanel::updateCurrentSettings(){
    guiView.loadStateIntoSettings(&currentSettings);
}














