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

#include "MainPanel.h"
#include "ofApp.h"
#include "SettingsManager.h"

ofApp* mMainAppPtr; //remove



#pragma mark - core funcs
//-------------------------------------------------
void MainPanel::setup(int x, int y, int width, int height){
    BasePanel::setup(x, y, width, height);
    guiView.setup(x, y, width, GUI_COMP_HEIGHT, this);
    
    mMainAppPtr = (ofApp*)ofGetAppPtr();
    
    //colors
    setBackgroundColor(ofColor(80));
    
    projects_dir.listDir("projects/");
    for(int i = 0; i < (int)projects_dir.size(); i++){
        ofLogVerbose()<<"Main Panel: " << projects_dir.getPath(i);
    }
    //fonts for text display
    ofTrueTypeFont::setGlobalDpi(72);
    verdana.load("gui_assets/fonts/verdana.ttf", 12, true, true);
    verdana.setLineHeight(13.0f);
    verdana.setLetterSpacing(1.037);
    fileInfoStr = "non file loaded";
    
    SettingsManager::getInstance().setMainPanelPtr(this);
}
//-------------------------------------------------
void MainPanel::update(){
    guiView.update();
}
//-------------------------------------------------
void MainPanel::draw(){
    if (!View::mustDrawNewFrame()){
        View::drawLoadedTexture();
        return;
    }
    if (_isHidden){ return; }
    View::draw();
    
    guiView.draw();
    drawFileInfo();
    
    View::loadViewInTexture();
}
//-------------------------------------------------
void MainPanel::exit(){}
//--------------------------------------------------------------
void MainPanel::keyPressed(int key){
    if(guiView.getFocused()){
        return;
    }
}

bool MainPanel::getFocused(){
    return (guiView.getFocused());
}

void MainPanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, GUI_COMP_HEIGHT);
}


void MainPanel::drawFileInfo(){
    
    ofPushStyle();
    
    string infoStr = "File name: "+ fileName + " | " + fileInfoStr + " | buffer size: " + ofToString(mMainAppPtr->config.bufferSize) + " | proj: "+ mMainAppPtr->config.projectDir;
    
    //draw string
    ofSetColor(ofColor::darkKhaki);
    verdana.drawString(infoStr,  10, _h - 7);
    
    ofPopStyle();
}


//-------------------------------------------------
#pragma mark - Filesystem funcs
//--------------------------------------------------------------
void MainPanel::openOpenFileDialog(){
    
    mMainAppPtr->stop();
    
    //Open the Open File Dialog
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a wav or mp3");
    //Check if the user opened a file
    if (openFileResult.bSuccess){
        ofLogVerbose("User selected a file");
        //We have a file, check it and process it
        processOpenFileSelection(openFileResult);
        
    }else {
        ofLogVerbose("User hit cancel");
    }
    
}
//--------------------------------------------------------------
void MainPanel::processOpenFileSelection(ofFileDialogResult openFileResult){
    
    ofLogVerbose("getName(): "  + openFileResult.getName());
    fileName = openFileResult.getName();
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    
    ofFile file (openFileResult.getPath());
    
    if (file.exists()){
        
        ofLogVerbose("The file exists - now checking the type via file extension");
        ofLogVerbose(file.getExtension());
        
        string fileExtension = ofToUpper(file.getExtension());
        
        //cout<<"upper-"<<fileExtension<<endl;
        
        
        if (fileExtension == "WAV" || fileExtension == "MP3") {
            ofLogVerbose("if"+fileExtension);
            mMainAppPtr->openAudioFile(openFileResult.getPath());
        }else{
            ofLogVerbose("File extension not compatible");
        }
    }
}





#pragma mark - Settings funcs
void MainPanel::applySettings(int fps, int buffersize, float bpm, string host, int port){
    currentSettings.frameRate = fps;
    currentSettings.bufferSize = buffersize;
    currentSettings.bpm = bpm;
    currentSettings.osc.host = host;
    currentSettings.osc.port = port;
    
    //!
    //TODO: Implement!
}

void MainPanel::loadSettings(MainPanelSettings& settings){
    currentSettings = settings;
    guiView.setStateFromSettings(settings);
}


void MainPanel::updateCurrentSettings(){
    guiView.loadStateIntoSettings(&currentSettings);
}

void MainPanel::saveSettings(){
    SettingsManager::getInstance().saveSettings();
}

void MainPanel::loadSettings(){
    SettingsManager::getInstance().loadSettings();
}














