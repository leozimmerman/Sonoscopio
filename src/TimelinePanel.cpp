
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
    
    setBackgroundColor(ofColor(50));
    bordCol = ofColor::grey;
    bordWidth = 1;
    waveformCol.set(160);
    
    currentTrackType = CURVES;
    currentTrackName = "";
    
    //-------------------------
    ofxTimeline::removeCocoaMenusFromGlut("Audio Waveform Example");
    timeline.setWorkingFolder(TIMELINE_SETTINGS_DIR);
    timeline.setup();
    timeline.setAutosave(TRUE);
    timeline.setOffset(ofVec2f(_x, _y));
   
    
    timeline.setLoopType(OF_LOOP_NONE);
    
    timeline.setBPM(120.f);
    timeline.setShowBPMGrid(false);
    
    
    
    //timeline.addAudioTrack("Audio","audio_files/rock.mp3");//stereo
    timeline.addAudioTrack("Audio","audio_files/flauta.wav");//mono
    //timeline.addAudioTrack("Audio","audio_files/mix-stereo.wav");//mono
    //timeline.addAudioTrack("Audio","audio_files/4chan.wav");//mono
    
    
    //this means that calls to play/stop etc will be  routed to the waveform and that timing will be 100% accurate
    timeline.setTimecontrolTrack("Audio");
    timeline.setDurationInSeconds(timeline.getAudioTrack("Audio")->getDuration());
    
    audioTrack = timeline.getAudioTrack("Audio");
    timeline.setPageName(PAGE_AUDIO_NAME);
    timeline.addPage(PAGE_TRACKS_NAME);
    addTrack("default", CURVES);
    timeline.setCurrentPage(PAGE_AUDIO_NAME);
    timeline.setShowPageTabs(false); //->modify if more pages are needed
    
    timeline.setWidth(_w);
    timeline.setHeight(_h);
    
    
    //-------------------------------------------
    
    ofAddListener(timeline.events().bangFired, this, &TimelinePanel::bangFired);
    
    setupGUI();
    
    
    
}
//-------------------------------------------------
void TimelinePanel::update(){
    
    //check for height changes
    int tl_h = timeline.getPage(PAGE_AUDIO_NAME)->getDrawRect().height + 66;
    if(tl_h != _h - guiCompHeight){
        resizeHeight(tl_h);
    }
    
    //update gui components
    for(int i=0; i<components.size(); i++) components[i]->update();


}
//-------------------------------------------------
void TimelinePanel::draw(){
    
    drawBackground();

    
    //draw gui------------------
    TS_START("gui");
    for(int i=0; i<components.size(); i++){
        components[i]->draw();
    }
    TS_STOP("gui");
    //draw timeline------------
    TS_START("timeline");
    timeline.draw();
    TS_STOP("timeline");
    
    //draw audio waveform in background----------
    //adjust with zoom
    TS_START("recompute");
    if(audioTrack->getShouldRecomputePreview() || audioTrack->getViewIsDirty()){
        audioTrack->recomputePreview();
    }
    TS_STOP("recompute");
    
    
    //draw waveforms
    TS_START("waveforms");
    ofPushStyle();
    ofSetColor(waveformCol);
    for(int i=0; i<audioTrack->getPreviews().size(); i++){
        audioTrack->getPreviews()[i].draw();
    }
    ofPopStyle();
    TS_STOP("waveforms");
    
  

}
//--------------------------------------------------------------
void TimelinePanel::keyPressed(int key){
    
    
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    
    switch (key) {
        case 'e':
            if(ftrack != NULL && timeline.getCurrentPageName()==PAGE_TRACKS_NAME){
                timeline.getPage(PAGE_TRACKS_NAME)->expandFocusedTrack();
            }
            break;
        case 'd':
            if(ftrack != NULL && timeline.getCurrentPageName()==PAGE_TRACKS_NAME){
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
#pragma mark - Settings funcs
//--------------------------------------------------------------
void TimelinePanel::loadSettings(string rootDir){
    timeline.loadTracksFromFolder(rootDir + TIMELINE_SETTINGS_DIR);
}
//--------------------------------------------------------------
void TimelinePanel::saveSettings(string rootDir){
    timeline.saveTracksToFolder(rootDir + TIMELINE_SETTINGS_DIR);
}
//--------------------------------------------------------------
#pragma mark - Timeline funcs
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
    
    timeline.setCurrentPage(PAGE_TRACKS_NAME);
    
    switch (type) {
        case CURVES:
            timeline.addCurves(name, ofRange(0, 1));
            break;
        case BANGS:
            timeline.addBangs(name);
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
    
    
    if(timeline.getCurrentPageName() == PAGE_AUDIO_NAME){

        timeline.setCurrentPage(PAGE_TRACKS_NAME);
        timeline.setFootersHidden(TRUE);
        adjustTracksHeight();
        
    }else if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME) {
    
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        timeline.setFootersHidden(false);
    
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::adjustTracksHeight(){
    
    if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME){
        
        int audioH = timeline.getPage(PAGE_AUDIO_NAME)->getComputedHeight();
        int tracksNum = timeline.getPage(PAGE_TRACKS_NAME)->getTracksNum();
        float margin  = timeline.getDrawRect().height -  timeline.getPage(PAGE_AUDIO_NAME)->getDrawRect().height -66;
        timeline.setHeight(audioH + 66 - 17.68*tracksNum);
        
        
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::bangFired(ofxTLBangEventArgs& args){
    //cout << "bang fired!" << args.flag << endl;
    cout << "bang fired!" << endl;
    _isThereBang = TRUE;
    _bangedTrack = args.track;
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
    guiCompHeight = component->getHeight();///--guiCompHeight
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    
    gui_x += guiCompWidth*2;
    vector<string> options = {"curves", "bangs", "switches"};
    dropdown = new ofxDatGuiDropdown("TRACK TYPE", options);
    dropdown->setPosition(gui_x, gui_y);
    dropdown->setWidth(guiCompWidth, 0.9);
    //dropdown->expand();
    dropdown->onDropdownEvent(this, &TimelinePanel::onDropdownEvent);
    component = dropdown;
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setStripeColor(bordCol);
    components.push_back(dropdown);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("ADD");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("REMOVE");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("SHOW TRACKS");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("ADJUST TRACKS");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
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
std::map<string, float> TimelinePanel::getTracksValues(){
    
    std::map<string, float> values;
    ofxTLPage* trPage = timeline.getPage(PAGE_TRACKS_NAME);
    
    for (int i=0; i<trPage->getTracksNum(); i++){
        
        for(ofxTLTrack* track : trPage->getTracks()){
            //set key
            string name = track->getName();
            string key = "TL-" + name;
            
            //set value
            float value;
            if(track->getTrackType()=="Curves"){
                value = timeline.getValue(name);
            }else if(track->getTrackType()=="Switches"){
                value = timeline.isSwitchOn(name);
            }else if(track->getTrackType()=="Bangs"){
                if(_bangedTrack!=NULL && _bangedTrack==track && _isThereBang){
                    value = 1.0;
                }else{
                    value = 0.0;
                }
                
            }
            //add key & value
            values[key]= value;
        }
        
    }
    
    return values;
}
//--------------------------------------------------------------
#pragma mark - gui listeners
//--------------------------------------------------------------

void TimelinePanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    //ofLogVerbose() << "onButtonEvent: " << e.target->getLabel() << "::" << e.enabled ;
    
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
    //ofLogVerbose() << "onDropdownEvent: " << e.child ;
    
    switch (e.child) {
            
        case 0:
            currentTrackType = CURVES;
            break;
        case 1:
            currentTrackType = BANGS;
            break;
        case 2:
            currentTrackType = SWITCHES;
            break;
            
        default:
            break;
    }
}
//--------------------------------------------------------------
void TimelinePanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
   // ofLogVerbose() << "onButtonEvent: " << e.text;
    currentTrackName = e.text;
}


