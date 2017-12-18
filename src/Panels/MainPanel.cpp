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

ofApp* mMainAppPtr;

#pragma mark - core funcs
//-------------------------------------------------
void MainPanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    mMainAppPtr = dynamic_cast<ofApp*>(appPtr);
    
    //colors
    setBackgroundColor(ofColor(25));
    bordCol = ofColor::grey;
    bordWidth = 1;
    fileinfoFontCol = ofColor::darkKhaki;
    
    _rows = 3;
    _columns = 5;
    
    projects_dir.listDir("projects/");
    for(int i = 0; i < (int)projects_dir.size(); i++){
        ofLogVerbose()<<"Main Panel: " << projects_dir.getPath(i);
    }
    
    setupGui();

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
    for(int i=0; i<components.size(); i++) components[i]->update();
}
//-------------------------------------------------
void MainPanel::draw(){
    
    drawBackground();
    
    for(int i=0; i<components.size(); i++) components[i]->draw();
    drawFileInfo();
    
}
//-------------------------------------------------
void MainPanel::exit(){

}

//-------------------------------------------------
void MainPanel::drawFileInfo(){
    
    ofPushStyle();
    
    string infoStr = "File name: "+ fileName + " | " + fileInfoStr + " | buffer size: " + ofToString(mMainAppPtr->getBufferSize()) + " | proj: "+ mMainAppPtr->getProjectDir();
    
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
#pragma mark - Settings funcs
//-------------------------------------------------
void MainPanel::loadSettings(string rootDir){
    
    ofxXmlSettings xml;
    string filename = rootDir + _panelDir + "/main_settings.xml";
    
    if( xml.loadFile(filename) ){
        ofLogVerbose()<<"MainPanel: "<< filename <<" loaded.";
    }else{
        ofLogError()<< "MainPanel: unable to load " << filename ;
        return;
    }
    
    //-----------
    float val = xml.getValue("PANEL:VOLUME", 0.0);
    gVolume->setValue(val);
    if (mMainAppPtr->timePanel.audioTrack != NULL)
        mMainAppPtr->timePanel.audioTrack->setVolume(val);
    //-----------
    bool state = xml.getValue("PANEL:SPLIT", 0) > 0;
    gSplit->setEnabled(state);
    if(state)
        mMainAppPtr->setAnalysisMode(SPLIT);
    else
        mMainAppPtr->setAnalysisMode(MONO);
    //-----------
    state = xml.getValue("PANEL:LOOP", 0) > 0;
    gLoop->setEnabled(state);
    if(state)
        mMainAppPtr->timePanel.timeline.setLoopType(OF_LOOP_NORMAL);
    else
        mMainAppPtr->timePanel.timeline.setLoopType(OF_LOOP_NONE);
    //-----------
    state = xml.getValue("PANEL:SEND-OSC", 0) > 0;
    gSendOsc->setEnabled(state);
    mMainAppPtr->setIsSendingOsc(state);
    //-----------
    state = xml.getValue("PANEL:BPM-GRID", 0) > 0;
    gShowBpm->setEnabled(state);
    mMainAppPtr->timePanel.timeline.setShowBPMGrid(state);
    //-----------
    state = xml.getValue("PANEL:SNAP-BPM", 0) > 0;
    gSnapBpm->setEnabled(state);
    mMainAppPtr->timePanel.timeline.enableSnapToBPM(state);
    //-----------
    state = xml.getValue("PANEL:FRAMEBASED", 0) > 0;
    gFramebased->setEnabled(state);
    mMainAppPtr->timePanel.timeline.setFrameBased(state);
    
    //-----------
    ///CONFIG MENU:
    //-----------
    string text = xml.getValue("PANEL:PORT", "");
    //gPort->setText(text);
    mMainAppPtr->setOscSenderPort( std::stoi(text) );
    //-----------
    text = xml.getValue("PANEL:BPM", "");
    //gBpm->setText(text);
    mMainAppPtr->timePanel.timeline.setNewBPM( std::stof (text) );
    //-----------
    text = xml.getValue("PANEL:HOST", "");
    //gHost->setText(text);
    mMainAppPtr->setOscSenderHost(text);
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
    savedSettings.addValue("FRAMERATE", mMainAppPtr->mMenu->getFpsText());
    savedSettings.addValue("BUFFER-SIZE", mMainAppPtr->mMenu->getBufferSizeText());
    savedSettings.addValue("BPM",  mMainAppPtr->mMenu->getBpmText());
    savedSettings.addValue("PORT", mMainAppPtr->mMenu->getPortText());
    savedSettings.addValue("HOST", mMainAppPtr->mMenu->getHostText());
    
    savedSettings.saveFile(filename);

    
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
//-------------------------------------------------
#pragma mark - GUI funcs
//-------------------------------------------------
void MainPanel::setupGui(){
    

    ofxDatGuiComponent* component;
    
    ///1stCol-------
    
    component = new ofxDatGuiButton("OPEN FILE");
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("LOAD SETTINGS");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SAVE SETTINGS");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    
    
    ///2ndCol--------
    component = new ofxDatGuiButton("CONFIG");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);

    component = new ofxDatGuiButton("SAVE ANALYSIS");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    //FRAMERATE MONITOR
    gFpsMonitor = new ofxDatGuiFRM(0.5f);//updates ever 0.5 secs.
    component = gFpsMonitor;
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    
   

    vector<string> options;
    for(int i = 0; i < (int)projects_dir.size(); i++){
        string option = projects_dir.getPath(i);
        option.erase(0, 9);//delete "projects/"
        options.push_back(option);
    }
    component = new ofxDatGuiDropdown("OPEN PROJECT", options);
    component->onDropdownEvent(this, &MainPanel::onProjectDropdownEvent);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    

    

    ///3rdCol--------
    gSplit = new ofxDatGuiToggle("CHANNELS SPLIT", false);//FIXME: link with ofApp setup, _currentAnalysisMode
    component = gSplit;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gSendOsc = new ofxDatGuiToggle("SEND OSC", TRUE);
    component = gSendOsc;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gFramebased = new ofxDatGuiToggle("FRAME BASED", false);
    component = gFramebased;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
   ///4thCol--------
    component = new ofxDatGuiButton("PLAY / STOP");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gVolume = new ofxDatGuiSlider("VOLUME", 0.0, 1.0, 1.0);
    component = gVolume;
    component->onSliderEvent(this, &MainPanel::onSliderEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gLoop = new ofxDatGuiToggle("LOOP ON-OFF", false);
    component = gLoop;
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    ///5thCol--------
    
    component = new ofxDatGuiButton("ADD MARKER");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("CLEAR MARKERS");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gShowBpm = new ofxDatGuiToggle("BPM GRID", false);
    component = gShowBpm;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setLabelMargin(10);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gSnapBpm = new ofxDatGuiToggle("SNAP", false);
    component = gSnapBpm;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SET IN");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SET OUT");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    

    //---------
    //-:SET POSITION, WIDTH AND HEIGHT OF COMPONENTS
    adjustGui(_w, _h);
   

}
//-------------------------------------------------
void MainPanel::resize(int w, int h){
    adjustGui(w,h);
}
//-------------------------------------------------
void MainPanel::adjustGui(int w, int h){
    
    //necessary?
    _w = w;
    _h = h;
    
    _guiCompWidth = _w / _columns;
    int guiCompWidth = _guiCompWidth;
    int guiCompHeight = (_h - FILE_INFO_HEIGHT) / _rows;
    
    if(components.size()==0){
        ofLogError()<<"Cant resize MainPanel, components size = 0";
        return;
    }
    
    ofxDatGuiComponent* component;
    
    ///1stCol--------
    //"OPEN FILE"
    component = components[0];
    component->setPosition(_x, _y+ guiCompHeight*0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    //LOAD SETTINGS
    component = components[1];
    component->setPosition(_x, _y + guiCompHeight * 1);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    //SAVE SETTINGS
    component = components[2];
    component->setPosition(_x, _y + guiCompHeight * 2);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);

    
    ///2ndCol--------
    //CONFIG
    component = components[3];
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight * 0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    //SAVE ANALYSIS
    component = components[4];
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight * 1);
    component->setWidth(guiCompWidth/2, 0.9);
    component->setHeight(guiCompHeight);
    
    //FRAMERATE MONITOR
    component = components[5];
    component->setPosition(_x + guiCompWidth * 1.5, _y + guiCompHeight * 1);
    component->setWidth(guiCompWidth/2, 0.65);
    component->setHeight(guiCompHeight);
    
    //OPEN PROJECT
    component = components[6];
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight * 2);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    ///3rdCol--------
    
    //CHANNEL SPLIT
    component = components[7];
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight * 0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    //SEND OSC
    component = components[8];
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight * 1);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    
    //FRAME BASED
    component = components[9];
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight * 2);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    ///4thCol--------
    
    
    //PLAY-STOP
    component = components[10];
    component->setPosition(_x + guiCompWidth * 3, _y + guiCompHeight*0);
    component->setWidth(guiCompWidth, 0.65);
    component->setHeight(guiCompHeight);
    
    //VOLUME
    component = components[11];
    component->setPosition(_x + guiCompWidth * 3, _y + guiCompHeight*1);
    component->setWidth(guiCompWidth, 0.4);
    component->setHeight(guiCompHeight);
    
    //LOOP ON-OFF
    component = components[12];
    component->setPosition(_x + guiCompWidth * 3, _y + guiCompHeight*2);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    
    ///5thCol--------
    
    //ADD MARKER
    component = components[13];
    component->setPosition(_x + guiCompWidth * 4, _y + guiCompHeight*0);
    component->setWidth(guiCompWidth/2, 0.9);
    component->setHeight(guiCompHeight);
    
    //CLEAR MARKER
    component = components[14];
    component->setPosition(_x + guiCompWidth * 4.5, _y + guiCompHeight*0);
    component->setWidth(guiCompWidth/2, 0.9);
    component->setHeight(guiCompHeight);
    
    //BPM GRID
    component = components[15];
    component->setPosition(_x + guiCompWidth * 4, _y + guiCompHeight*1);
    component->setWidth(guiCompWidth/2, 0.9);
    component->setHeight(guiCompHeight);
    
    //SNAP TO BPM
    component = components[16];
    component->setPosition(_x + guiCompWidth * 4.5, _y + guiCompHeight * 1);
    component->setWidth(guiCompWidth/2, 0.9);
    component->setHeight(guiCompHeight);
    
    //SET-IN
    component = components[17];
    component->setPosition(_x + guiCompWidth * 4, _y + guiCompHeight * 2);
    component->setWidth(guiCompWidth/2, 0.9);
    component->setHeight(guiCompHeight);
   
    
    //SET-OUT
    component = components[18];
    component->setPosition(_x + guiCompWidth * 4.5, _y + guiCompHeight * 2);
    component->setWidth(guiCompWidth/2, 0.9);
    component->setHeight(guiCompHeight);
    
}
//-------------------------------------------------
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
//-------------------------------------------------
#pragma mark - Listeners
//-------------------------------------------------
void MainPanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
   // cout << "MainPanel-onButtonEvent: " << e.target->getLabel() << "::" << e.enabled << endl;
    
    
    if(e.target->getLabel()=="OPEN FILE"){
        
        openOpenFileDialog();
    
    }else if(e.target->getLabel()=="LOAD SETTINGS"){
        
        mMainAppPtr->loadSettings();
        
    }else if(e.target->getLabel()=="SAVE SETTINGS"){
        
        mMainAppPtr->saveSettings();
        
    }else if(e.target->getLabel()=="FRAME BASED"){
        
        mMainAppPtr->timePanel.timeline.setFrameBased(e.enabled);
        
    }else if(e.target->getLabel()=="LOOP ON-OFF"){
        
        if(e.enabled)
            mMainAppPtr->timePanel.timeline.setLoopType(OF_LOOP_NORMAL);
        else
            mMainAppPtr->timePanel.timeline.setLoopType(OF_LOOP_NONE);
        
    }else if(e.target->getLabel()=="CHANNELS SPLIT"){
        
        if(e.enabled)
            mMainAppPtr->setAnalysisMode(SPLIT);
        else
            mMainAppPtr->setAnalysisMode(MONO);
        
    }else if(e.target->getLabel()=="PLAY / STOP"){
        
        mMainAppPtr->togglePlay();
        
    }else if(e.target->getLabel()=="SET IN"){
        
        mMainAppPtr->timePanel.timeline.setInPointAtPlayhead();
        
    }else if(e.target->getLabel()=="SET OUT"){
        
        mMainAppPtr->timePanel.timeline.setOutPointAtPlayhead();
    
    }else if(e.target->getLabel()== "SEND OSC"){
        
        mMainAppPtr->setIsSendingOsc(e.enabled);
        
    }else if(e.target->getLabel()== "BPM GRID"){
        
        mMainAppPtr->timePanel.timeline.setShowBPMGrid(e.enabled);
    
    }else if(e.target->getLabel()== "SNAP"){
        
        mMainAppPtr->timePanel.timeline.enableSnapToBPM(e.enabled);
    
    }else if(e.target->getLabel()== "SAVE ANALYSIS"){
        
        mMainAppPtr->saveAnalysisDataToFile();
        
    }
    else if(e.target->getLabel()== "FULL SCREEN"){
        
        ofToggleFullscreen();
        
    }
    else if(e.target->getLabel()== "ADD MARKER"){
        
        mMainAppPtr->timePanel.addMarker();
        
    }
    else if(e.target->getLabel()== "CLEAR MARKERS"){
        
        mMainAppPtr->timePanel.clearMarkers();
        
    }
    else if(e.target->getLabel()== "CONFIG"){
        
        mMainAppPtr->showMenu();
        
    }

    
    
  
}


//--------------------------------------------------------------

void MainPanel::onTextInputEvent(ofxDatGuiTextInputEvent e){
    //cout << "onButtonEvent: " << e.text << endl;
    //cout << "onTextInput: " << e.text << endl;
    if (e.target->getLabel()=="BPM"){
        try{
            mMainAppPtr->timePanel.timeline.setNewBPM( std::stof (e.text) );
        }
        catch (const std::invalid_argument& ia) {
            e.target->setText("ERROR");
            std::cerr << "Invalid BPM: " << ia.what() << '\n';
        }
    }else if (e.target->getLabel()=="HOST"){
        
        mMainAppPtr->setOscSenderHost(e.text);
        
    }else if (e.target->getLabel()=="PORT"){
        try{
            mMainAppPtr->setOscSenderPort( std::stoi(e.text) );
        }
        catch (const std::invalid_argument& ia) {
            e.target->setText("ERROR");
            std::cerr << "Invalid PORT: " << ia.what() << '\n';
        }
    }else if (e.target->getLabel()=="SET FPS"){
        try{
            mMainAppPtr->setFrameRate( std::stoi(e.text) );
        }
        catch (const std::invalid_argument& ia) {
            e.target->setText("ERROR");
            std::cerr << "Invalid FPS: " << ia.what() << '\n';
        }
    }
    
}
//--------------------------------------------------------------
void MainPanel::onSliderEvent(ofxDatGuiSliderEvent e){
    //cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
    if (e.target->getLabel()=="VOLUME"){
        if (mMainAppPtr->timePanel.audioTrack != NULL)
            mMainAppPtr->timePanel.audioTrack->setVolume(e.value);
    }
}

//--------------------------------------------------------------
void MainPanel::onProjectDropdownEvent(ofxDatGuiDropdownEvent e)
{
   // ofLogVerbose() << "onDropdownEvent: " << e.child << "--"<<e.target->getLabel()<<"--"<<e.parent;
    
    string projectDir = projects_dir.getPath(e.child);
    mMainAppPtr->openProject(projectDir);
    

}
//--------------------------------------------------------------
void MainPanel::onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e)
{
    // ofLogVerbose() << "onDropdownEvent: " << e.child << "--"<<e.target->getLabel()<<"--"<<e.parent;
    
    switch (e.child) {
            
        case 0:
            mMainAppPtr->setBufferSize(256);
            break;
        case 1:
            mMainAppPtr->setBufferSize(512);
            break;
        case 2:
            mMainAppPtr->setBufferSize(1024);
            break;
        case 3:
            mMainAppPtr->setBufferSize(2048);
            break;
            
        default:
            break;
    }
        
    
    
    
}


