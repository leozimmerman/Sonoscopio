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

ofApp* mMainAppPtr; //re

#pragma mark - core funcs
//-------------------------------------------------
void MainPanel::setup(int x, int y, int width, int height){
    BasePanel::setup(x, y, width, height);
    guiView.setup(x, y, width, GUI_COMP_HEIGHT, this);
    //guiView.setup(x, y, width, GUI_COMP_HEIGHT, this);
    
    mMainAppPtr = (ofApp*)ofGetAppPtr();
    
    //colors
    setBackgroundColor(ofColor(80));

    fileinfoFontCol = ofColor::darkKhaki;
    
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
    
    _panelDir = MAIN_SETTINGS_DIR;

  
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
    return false;
    //    if(gHost->getFocused() ||
    //       gPort->getFocused() ||
    //       gBpm->getFocused()  ||
    //       gFps->getFocused()   )
    //    {
    //        return true;
    //    }else{
    //        return false;
    //    }
}

void MainPanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, GUI_COMP_HEIGHT);
}


void MainPanel::drawFileInfo(){
    
    ofPushStyle();
    
    string infoStr = "File name: "+ fileName + " | " + fileInfoStr + " | buffer size: " + ofToString(mMainAppPtr->config.getBufferSize()) + " | proj: "+ mMainAppPtr->config.getProjectDir();
    
    //draw boundbox
    
//    ofRectangle boundBox = verdana.getStringBoundingBox(infoStr,  _w-guiCompWidth, _h * 0.15);
//    ofSetColor(40);
//    ofDrawRectangle(boundBox);
    
    //draw string
    ofSetColor(fileinfoFontCol);
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

void MainPanel::loadSettings(string rootDir){
    
    ofxXmlSettings xml;
    string filename = rootDir + _panelDir + "/main_settings.xml";
    
    if( xml.loadFile(filename) ){
        ofLogVerbose()<<"MainPanel: "<< filename <<" loaded.";
    }else{
        ofLogError()<< "MainPanel: unable to load " << filename ;
        return;
    }
    
    /**
     float val = xml.getValue("PANEL:VOLUME", 0.0);
     gVolume->setValue(val);
     ///mMainAppPtr->timePanel.setVolume(val);
     //-----------
     //TODO: Remove
     bool state = xml.getValue("PANEL:SPLIT", 0) > 0;
     gSplit->setEnabled(state);
     //-----------
     state = xml.getValue("PANEL:LOOP", 0) > 0;
     gLoop->setEnabled(state);
     ///if(state)
     ///mMainAppPtr->timePanel.setLoopType(OF_LOOP_NORMAL);
     ///else
     ///mMainAppPtr->timePanel.setLoopType(OF_LOOP_NONE);
     //-----------
     state = xml.getValue("PANEL:SEND-OSC", 0) > 0;
     gSendOsc->setEnabled(state);
     mMainAppPtr->config.setIsSendingOsc(state);
     //-----------
     state = xml.getValue("PANEL:BPM-GRID", 0) > 0;
     gShowBpm->setEnabled(state);
     ///mMainAppPtr->timePanel.setShowBPMGrid(state);
     //-----------
     state = xml.getValue("PANEL:SNAP-BPM", 0) > 0;
     gSnapBpm->setEnabled(state);
     ///mMainAppPtr->timePanel.enableSnapToBPM(state);
     //-----------
     state = xml.getValue("PANEL:FRAMEBASED", 0) > 0;
     gFramebased->setEnabled(state);
     ///mMainAppPtr->timePanel.setFrameBased(state);
     */
    //-----------
    ///CONFIG MENU:
    //-----------
    string text = xml.getValue("PANEL:PORT", "");
    //gPort->setText(text);
    mMainAppPtr->oscSender.setPort( std::stoi(text) );
    //-----------
    text = xml.getValue("PANEL:BPM", "");
    //gBpm->setText(text);
    ///mMainAppPtr->timePanel.setNewBPM( std::stof (text) );
    //-----------
    text = xml.getValue("PANEL:HOST", "");
    //gHost->setText(text);
    mMainAppPtr->oscSender.setHost(text);
    //-----------
    text = xml.getValue("PANEL:FRAMERATE", "");
    //gFps->setText(text);
    mMainAppPtr->setFrameRate( std::stoi(text) );
    //-----------
    text = xml.getValue("PANEL:BUFFER-SIZE", "");
    int bs = std::stoi(text);
    switch (bs) {
        case 256:
            //gBufferSize->select(0);
            mMainAppPtr->setBufferSize(256);
            break;
        case 512:
            //gBufferSize->select(1);
            mMainAppPtr->setBufferSize(512);
            break;
        case 1024:
            //gBufferSize->select(2);
            mMainAppPtr->setBufferSize(1024);
            break;
        case 2048:
            //gBufferSize->select(3);
            mMainAppPtr->setBufferSize(2048);
            break;
            
        default:
            break;
    }
    
    //-----------
    
    
}
//-------------------------------------------------
void MainPanel::saveSettings(string rootDir){
    
    string filename = rootDir + _panelDir + "/main_settings.xml";
    
    ofxXmlSettings savedSettings;
    /**
     savedSettings.addTag("PANEL");
     savedSettings.pushTag("PANEL");
     savedSettings.addValue("VOLUME", gVolume->getValue());
     savedSettings.addValue("SPLIT", gSplit->getEnabled());
     savedSettings.addValue("LOOP", gLoop->getEnabled());
     savedSettings.addValue("SEND-OSC", gSendOsc->getEnabled());
     savedSettings.addValue("BPM-GRID", gShowBpm->getEnabled());
     savedSettings.addValue("SNAP-BPM", gSnapBpm->getEnabled());
     savedSettings.addValue("FRAMEBASED", gFramebased->getEnabled());
     
     //Config menu settings:
     
     savedSettings.addValue("FRAMERATE", menuModal->getFpsText());
     savedSettings.addValue("BUFFER-SIZE", menuModal->getBufferSizeText());
     savedSettings.addValue("BPM",  menuModal->getBpmText());
     savedSettings.addValue("PORT", menuModal->getPortText());
     savedSettings.addValue("HOST", menuModal->getHostText());
     */
    savedSettings.saveFile(filename);
    
    
}














