
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
    setBackgroundColor(ofColor::white);
    bordCol = ofColor::grey;
    bordWidth = 1;
    fileinfoFontCol = ofColor::black;
    
    
    projects_dir.listDir("projects/");
    for(int i = 0; i < (int)projects_dir.size(); i++){
        ofLogVerbose()<<"Main Panel: " << projects_dir.getPath(i);
    }
    
    //datgui------------
    setupGUI();

    //fonts for text display
    ofTrueTypeFont::setGlobalDpi(72);
    verdana.load("fonts/verdana.ttf", 12, true, true);
    verdana.setLineHeight(14.0f);
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
    
    string infoStr = "INFO:" "\nfile name: "+ fileName +
                        "\n" + fileInfoStr +
                        "\nproj: "+ mMainAppPtr->getProjectDir();
    
    //draw boundbox
    
//    ofRectangle boundBox = verdana.getStringBoundingBox(infoStr,  _w-guiCompWidth, _h * 0.15);
//    ofSetColor(40);
//    ofDrawRectangle(boundBox);
    
    //draw string
    ofSetColor(fileinfoFontCol);
    verdana.drawString(infoStr,  _w-guiCompWidth, _h * 0.15);
    
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
    
    float val = xml.getValue("PANEL:VOLUME", 0.0);
    gVolume->setValue(val);
    if (mMainAppPtr->timePanel.audioTrack != NULL)
        mMainAppPtr->timePanel.audioTrack->setVolume(val);
    //--
    bool state = xml.getValue("PANEL:SPLIT", 0) > 0;
    gSplit->setEnabled(state);
    if(state)
        mMainAppPtr->setAnalysisMode(SPLIT);
    else
        mMainAppPtr->setAnalysisMode(MONO);
    //--
    state = xml.getValue("PANEL:LOOP", 0) > 0;
    gLoop->setEnabled(state);
    if(state)
        mMainAppPtr->timePanel.timeline.setLoopType(OF_LOOP_NORMAL);
    else
        mMainAppPtr->timePanel.timeline.setLoopType(OF_LOOP_NONE);
    //-
    state = xml.getValue("PANEL:SEND-OSC", 0) > 0;
    gSendOsc->setEnabled(state);
    mMainAppPtr->setIsSendingOsc(state);
    //-
    string text = xml.getValue("PANEL:HOST", "");
    gHost->setText(text);
    mMainAppPtr->setOscSenderHost(text);
    //-
    text = xml.getValue("PANEL:PORT", "");
    gPort->setText(text);
    mMainAppPtr->setOscSenderPort( std::stoi(text) );
    //-
    state = xml.getValue("PANEL:SHOW-BPM", 0) > 0;
    gShowBpm->setEnabled(state);
    mMainAppPtr->timePanel.timeline.setShowBPMGrid(state);
    //-
    text = xml.getValue("PANEL:BPM", "");
    gBpm->setText(text);
    mMainAppPtr->timePanel.timeline.setNewBPM( std::stof (text) );
    //-
    state = xml.getValue("PANEL:SNAP-BPM", 0) > 0;
    gSnapBpm->setEnabled(state);
    mMainAppPtr->timePanel.timeline.enableSnapToBPM(state);
    //-
    state = xml.getValue("PANEL:FRAMEBASED", 0) > 0;
    gFramebased->setEnabled(state);
    mMainAppPtr->timePanel.timeline.setFrameBased(state);
    
    
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
    savedSettings.addValue("HOST", gHost->getText());
    savedSettings.addValue("PORT", gPort->getText());
    savedSettings.addValue("SHOW-BPM", gShowBpm->getEnabled());
    savedSettings.addValue("BPM", gBpm->getText());
    savedSettings.addValue("SNAP-BPM", gSnapBpm->getEnabled());
    savedSettings.addValue("FRAMEBASED", gFramebased->getEnabled());
    
    savedSettings.saveFile(filename);

    
}
//-------------------------------------------------
#pragma mark - Filesystem funcs
//--------------------------------------------------------------
void MainPanel::openOpenFileDialog(){
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
        
        cout<<"upper-"<<fileExtension<<endl;
        
        
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
void MainPanel::setupGUI(){
    guiCompWidth = _w / 6;
    guiCompHeight = _h / 2;
    ofxDatGuiComponent* component;
    
    component = new ofxDatGuiButton("OPEN FILE");
    component->setPosition(_x, _y+ guiCompHeight*0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
 
    
    component = new ofxDatGuiButton("LOAD SETTINGS");
    component->setPosition(_x, _y + guiCompHeight*0.5);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SAVE SETTINGS");
    component->setPosition(_x, _y + guiCompHeight*1.0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
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
    component->setPosition(_x, _y+ guiCompHeight*1.5);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    
    components.push_back(component);
    
    //--------
    
    component = new ofxDatGuiButton("PLAY / STOP");
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight*0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gVolume = new ofxDatGuiSlider("VOLUME", 0.0, 1.0, 1.0);
    component = gVolume;
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight*0.5);
    component->setWidth(guiCompWidth, 0.4);
    component->setHeight(guiCompHeight/2);
    component->onSliderEvent(this, &MainPanel::onSliderEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gSplit = new ofxDatGuiToggle("CHANNELS SPLIT", TRUE);
    component = gSplit;
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight*1.0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    vector<string> buff_sizes = {"256", "512", "1024", "2048"};
    component = new ofxDatGuiDropdown("BUFFER SIZE", buff_sizes);
    component->onDropdownEvent(this, &MainPanel::onBufferSizeDropdownEvent);
    component->setPosition(_x + guiCompWidth, _y+ guiCompHeight*1.5);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    
    components.push_back(component);
    
    //---------
    
    gLoop = new ofxDatGuiToggle("LOOP ON-OFF", false);
    component = gLoop;
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight*0);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SET IN");
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight*1);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SET OUT");
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight*1.5);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    //---------
    gSendOsc = new ofxDatGuiToggle("SEND OSC", TRUE);
    component = gSendOsc;
    component->setPosition(_x + guiCompWidth *3, _y + guiCompHeight*0);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(_h/3);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gHost = new ofxDatGuiTextInput("HOST", "localhost");
    component = gHost;
    component->setPosition(_x + guiCompWidth * 3, _y + _h/3);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.35);
    component->setHeight(_h/3);
    component->onTextInputEvent(this, &MainPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gPort = new ofxDatGuiTextInput("PORT", "12345");
    component = gPort;
    component->setPosition(_x + guiCompWidth * 3, _y + (_h/3)*2);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.35);
    component->setHeight(_h/3);
    component->onTextInputEvent(this, &MainPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    //---------
    gShowBpm = new ofxDatGuiToggle("SHOW BPM GRID", false);
    component = gShowBpm;
    component->setPosition(_x + guiCompWidth *4, _y + guiCompHeight*0);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gBpm = new ofxDatGuiTextInput("BPM", "120");
    component = gBpm;
    component->setPosition(_x + guiCompWidth * 4, _y + guiCompHeight*0.5);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.35);
    component->setHeight(guiCompHeight/2);
    component->onTextInputEvent(this, &MainPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gSnapBpm = new ofxDatGuiToggle("SNAP TO BPM", false);
    component = gSnapBpm;
    component->setPosition(_x + guiCompWidth *4, _y + guiCompHeight*1);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gFramebased = new ofxDatGuiToggle("FRAME BASED", false);
    component = gFramebased;
    component->setPosition(_x + guiCompWidth *4, _y + guiCompHeight*1.5);
    component->setWidth(guiCompWidth, 0.9);
    component->setHeight(guiCompHeight/2);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    //---------
    
    
    
   

}
//-------------------------------------------------
void MainPanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "MainPanel-onButtonEvent: " << e.target->getLabel() << "::" << e.enabled << endl;
    
    
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
        
        mMainAppPtr->startStopPlaying();
        
    }else if(e.target->getLabel()=="SET IN"){
        
        mMainAppPtr->timePanel.timeline.setInPointAtPlayhead();
        
    }else if(e.target->getLabel()=="SET OUT"){
        
        mMainAppPtr->timePanel.timeline.setOutPointAtPlayhead();
        
    }else if(e.target->getLabel()== "SEND OSC"){
        
        mMainAppPtr->setIsSendingOsc(e.enabled);
        
    }else if(e.target->getLabel()== "SHOW BPM GRID"){
        
        mMainAppPtr->timePanel.timeline.setShowBPMGrid(e.enabled);
    
    }else if(e.target->getLabel()== "SNAP TO BPM"){
        
        mMainAppPtr->timePanel.timeline.enableSnapToBPM(e.enabled);
    
    }
    
  
}


//--------------------------------------------------------------

void MainPanel::onTextInputEvent(ofxDatGuiTextInputEvent e){
    //cout << "onButtonEvent: " << e.text << endl;
    //cout << "onTextInput: " << e.text << endl;
    if (e.target->getLabel()=="BPM"){
        
        mMainAppPtr->timePanel.timeline.setNewBPM( std::stof (e.text) );
        
    }else if (e.target->getLabel()=="HOST"){
        
        mMainAppPtr->setOscSenderHost(e.text);
        
    }else if (e.target->getLabel()=="PORT"){
        
        mMainAppPtr->setOscSenderPort( std::stoi(e.text) );
        
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


