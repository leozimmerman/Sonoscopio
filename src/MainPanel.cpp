
#include "MainPanel.h"
#include "ofApp.h" 

ofApp* mMainApp;

//-------------------------------------------------

void MainPanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    mMainApp = dynamic_cast<ofApp*>(appPtr);
    
    setBackgroundColor(ofColor::darkRed);
    
    //datgui------------
    guiCompWidth = _w * 0.15;
    ofxDatGuiComponent* component;
    
    component = new ofxDatGuiButton("OPEN FILE");
    component->setPosition(_x, _y+ guiCompHeight*0);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    guiCompHeight = component->getHeight();
    components.push_back(component);
    
    component = new ofxDatGuiButton("LOAD");
    component->setPosition(_x, _y + guiCompHeight*1);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SAVE");
    component->setPosition(_x, _y + guiCompHeight*2);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    //--------
    
    component = new ofxDatGuiToggle("FRAME BASED", false);
    component->setPosition(_x + guiCompWidth, _y);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiToggle("LOOP ON-OFF", false);
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiToggle("CHANNELS SPLIT", false);
    component->setPosition(_x + guiCompWidth, _y + guiCompHeight*2);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    //---------
    
    component = new ofxDatGuiButton("PLAY / STOP");
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight*0);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SET IN");
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight*1);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SET OUT");
    component->setPosition(_x + guiCompWidth * 2, _y + guiCompHeight*2);
    component->setWidth(guiCompWidth, 0.9);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    //---------
    component = new ofxDatGuiToggle("SEND OSC", false);
    component->setPosition(_x + guiCompWidth *3, _y + guiCompHeight*0);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiTextInput("HOST", "#osc host#");
    component->setPosition(_x + guiCompWidth * 3, _y + guiCompHeight*1);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.35);
    component->onTextInputEvent(this, &MainPanel::onTextInputEvent);
    components.push_back(component);

    component = new ofxDatGuiTextInput("PORT", "#osc port#");
    component->setPosition(_x + guiCompWidth * 3, _y + guiCompHeight*2);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.35);
    component->onTextInputEvent(this, &MainPanel::onTextInputEvent);
    components.push_back(component);
    
    //---------
    component = new ofxDatGuiToggle("SHOW BPM GRID", false);
    component->setPosition(_x + guiCompWidth *4, _y + guiCompHeight*0);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &MainPanel::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiTextInput("BPM", "#bpm#");
    component->setPosition(_x + guiCompWidth * 4, _y + guiCompHeight*1);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth, 0.35);
    component->onTextInputEvent(this, &MainPanel::onTextInputEvent);
    components.push_back(component);
    
    
    
    

    //fonts for text display
    ofTrueTypeFont::setGlobalDpi(72);
    verdana14.load("verdana.ttf", 12, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
    
    fileInfoStr = "non file loaded";
  
    
 
    
    
}
//-------------------------------------------------
void MainPanel::update(){
    for(int i=0; i<components.size(); i++) components[i]->update();
}
//-------------------------------------------------
void MainPanel::draw(){
    
    ofPushStyle();
    ofSetColor(getBackgroundColor());
    ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    for(int i=0; i<components.size(); i++) components[i]->draw();
    
    
    ofPushStyle();
    ofSetColor(ofColor::white);
    string infoStr = "FILE INFO:" "\nfile name: "+ fileName +"\n" + fileInfoStr;
    verdana14.drawString(infoStr,  _w-guiCompWidth, _h * 0.1);
    ofPopStyle();
   
    
    
}
//-------------------------------------------------
void MainPanel::exit(){

}
//-------------------------------------------------
void MainPanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "MainPanel-onButtonEvent: " << e.target->getLabel() << "::" << e.enabled << endl;
    
    if(e.target->getLabel()=="OPEN FILE"){
        
        openOpenFileDialog();
    
    }else if(e.target->getLabel()=="PLAY / STOP"){
        
        mMainApp->startStopPlaying();
    }
    
  
}

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
            mMainApp->openAudioFile(openFileResult.getPath());
        }else{
            ofLogVerbose("File extension not compatible");
        }
        
    }
    
    
}

void MainPanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "onButtonEvent: " << e.text << endl;
}


