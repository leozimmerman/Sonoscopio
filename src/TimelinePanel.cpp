
#include "TimelinePanel.h"

#include "ofApp.h"

ofApp* tMainApp;
ofxDatGuiDropdown* dropdown;

#pragma mark - Core funcs

//-------------------------------------------------

void TimelinePanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    
    tMainApp = dynamic_cast<ofApp*>(appPtr);
    
    setBackgroundColor(ofColor::darkBlue);
    
    currentTrackType = CURVES;
    currentTrackName = "";
    
    //-------------------------
    ofxTimeline::removeCocoaMenusFromGlut("Audio Waveform Example");
    
    timeline.setup();
    timeline.setOffset(ofVec2f(_x, _y));
    timeline.setWidth(_w);
    timeline.setHeight(_h);
    timeline.setLoopType(OF_LOOP_NONE);
    
    timeline.setBPM(120.f);
    timeline.setShowBPMGrid(false);
    
    timeline.addAudioTrack("Audio","audio_files/rock.mp3");
    //this means that calls to play/stop etc will be  routed to the waveform and that timing will be 100% accurate
    timeline.setTimecontrolTrack("Audio");
    timeline.setDurationInSeconds(timeline.getAudioTrack("Audio")->getDuration());
    
    audioTrack = timeline.getAudioTrack("Audio");
    timeline.setPageName("Page-Audio");
    timeline.addPage("Page-Tracks");
    addTrack("default", CURVES);
    timeline.setCurrentPage("Page-Audio");
    timeline.setShowPageTabs(false); //->modify if more pages are needed
    
    //-------------------------------------------
    
    setupGUI();

    
}
//-------------------------------------------------
void TimelinePanel::update(){
    
    //check for height changes
    int tl_h = timeline.getPage("Page-Audio")->getDrawRect().height + 66;
    if(tl_h != _h - guiCompHeight){
        resizeHeight(tl_h);
    }
    
    //update gui components
    for(int i=0; i<components.size(); i++) components[i]->update();


}
//-------------------------------------------------
void TimelinePanel::draw(){
    
    //background
    ofPushStyle();
        ofSetColor(getBackgroundColor());
        ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    
    
    
    //draw gui------------------
    for(int i=0; i<components.size(); i++) components[i]->draw();
    
    //draw timeline------------
    timeline.draw();
    
    //draw audio waveform in background----------
    //adjust with zoom
    if(audioTrack->getShouldRecomputePreview() || audioTrack->getViewIsDirty()){
        audioTrack->recomputePreview();
    }
    //draw waveforms
    for(int i=0; i<audioTrack->getPreviews().size(); i++){
        audioTrack->getPreviews()[i].draw();
    }
    
  

}
//--------------------------------------------------------------
void TimelinePanel::keyPressed(int key){
    
    
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    
    switch (key) {
        case 'e':
            if(ftrack != NULL && timeline.getCurrentPageName()=="Page-Tracks"){
                timeline.getPage("Page-Tracks")->expandFocusedTrack();
            }
            break;
        case 'd':
            if(ftrack != NULL && timeline.getCurrentPageName()=="Page-Tracks"){
                ftrack->isEnabled() ? ftrack->disable() : ftrack->enable();
            }
            break;
        case 'a':
            adjustTracksHeight();
            break;
            
            
        default:
            break;
    }
}
//--------------------------------------------------------------


#pragma mark - Timeline funces
//--------------------------------------------------------------
void TimelinePanel::openAudioFile(string filename){

    timeline.stop();
    timeline.setCurrentTimeSeconds(0.0);
    audioTrack->loadSoundfile(filename);
    timeline.setDurationInSeconds(audioTrack->getDuration());
    
}
//--------------------------------------------------------------
void TimelinePanel::startStopPlaying(){
    if(timeline.getIsPlaying()){
        timeline.stop();
    }else{
        timeline.play();
    }
    
}

//--------------------------------------------------------------
void TimelinePanel::addTrack(string name, trackType type){
    
    timeline.setCurrentPage("Page-Tracks");
    
    switch (type) {
        case CURVES:
            timeline.addCurves(name, ofRange(0, 1));
            break;
        case BANGS:
            timeline.addBangs(name);
            break;
        case FLAGS:
            timeline.addFlags(name);
            break;
        case SWITCHES:
            timeline.addSwitches(name);
            
        default:
            break;
    }
    
    adjustTracksHeight();
    
    
}
//--------------------------------------------------------------
void TimelinePanel::removeTrack(string name){
    
    timeline.removeTrack(name);
    adjustTracksHeight();
}

//--------------------------------------------------------------
void TimelinePanel::toggleShowTracks(){
    
    
    if(timeline.getCurrentPageName() == "Page-Audio"){

        timeline.setCurrentPage("Page-Tracks");
        timeline.setFootersHidden(TRUE);
        adjustTracksHeight();
        
    }else if (timeline.getCurrentPageName() == "Page-Tracks") {
    
        timeline.setCurrentPage("Page-Audio");
        timeline.setFootersHidden(false);
    
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::adjustTracksHeight(){
    
    if (timeline.getCurrentPageName() == "Page-Tracks"){
        
        int audioH = timeline.getPage("Page-Audio")->getComputedHeight();
        int tracksNum = timeline.getPage("Page-Tracks")->getTracksNum();
        float margin  = timeline.getDrawRect().height -  timeline.getPage("Page-Audio")->getDrawRect().height -66;
        timeline.setHeight(audioH + 66 - 17.68*tracksNum);
        
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::toggleEnableDisableFocusedTrack(){

}
//--------------------------------------------------------------
#pragma mark - Utils
//--------------------------------------------------------------
void TimelinePanel::setupGUI(){
    
    ofxDatGuiComponent* component;
    guiCompWidth = _w / 7;
    
    
    int gui_y = _y + timeline.getHeight();
    int gui_x = _x;
    
    component = new ofxDatGuiTextInput("TRACK NAME", "#track name#");
    component->setPosition(gui_x, gui_y);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setWidth(guiCompWidth * 2, 0.35);//width x2
    component->onTextInputEvent(this, &TimelinePanel::onTextInputEvent);
    guiCompHeight = component->getHeight();//--
    components.push_back(component);
    
    
    gui_x += guiCompWidth*2;
    vector<string> options = {"curves", "bangs", "flags", "switches"};
    dropdown = new ofxDatGuiDropdown("TRACK TYPE", options);
    dropdown->setPosition(gui_x, gui_y);
    dropdown->setWidth(guiCompWidth, 0.9);
    //dropdown->expand();
    dropdown->onDropdownEvent(this, &TimelinePanel::onDropdownEvent);
    components.push_back(dropdown);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("ADD");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("REMOVE");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("SHOW TRACKS");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("ADJUST TRACKS");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    components.push_back(component);
    
    
    
}
//--------------------------------------------------------------
string TimelinePanel::getFileInfo(){
    
    string s;
    
    s =
    "duration: " + ofToString(audioTrack->getDuration(), 2) + "sec."
    "\nsample rate: " + ofToString(audioTrack->getSampleRate()) +
    "\nchannels: " + ofToString(audioTrack->getNumChannels()) +
    "\nbuffer size: " + ofToString(audioTrack->getBufferSize());
    
    return s;
    
}
//--------------------------------------------------------------
void TimelinePanel::resizeHeight(int tl_h){
    
    _h = tl_h + guiCompHeight;
    
    for(int i=0; i<components.size(); i++){
        int gui_y = _y + tl_h;
        components[i]->setPosition(components[i]->getX(), gui_y);
    }
    
    ofNotifyEvent(heightResizedEvent, _h, this);
}
//--------------------------------------------------------------
#pragma mark - gui listeners
//--------------------------------------------------------------

void TimelinePanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    ofLogVerbose() << "onButtonEvent: " << e.target->getLabel() << "::" << e.enabled ;
    
    string label = e.target->getLabel();
    if(label ==  "ADD"){
        
        addTrack(currentTrackName, currentTrackType);
    
    }else if (label == "REMOVE"){
    
        removeTrack(currentTrackName);
    
    }else if (label == "SHOW TRACKS"){
        
        toggleShowTracks();
        
    }else if (label == "ADJUST TRACKS"){
        
        adjustTracksHeight();
        
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    ofLogVerbose() << "onDropdownEvent: " << e.child ;
    
    switch (e.child) {
            
        case 0:
            currentTrackType = CURVES;
            break;
        case 1:
            currentTrackType = BANGS;
            break;
        case 2:
            currentTrackType = FLAGS;
            break;
        case 3:
            currentTrackType = SWITCHES;
            break;
            
        default:
            break;
    }
}
//--------------------------------------------------------------
void TimelinePanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    ofLogVerbose() << "onButtonEvent: " << e.text;
    currentTrackName = e.text;
}


