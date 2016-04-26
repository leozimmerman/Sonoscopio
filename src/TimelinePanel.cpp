
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
    page = timeline.getPage(timeline.getCurrentPageName());
    
    ///-------------------------------------------
    ofxDatGuiComponent* component;
    guiCompWidth = _w / 8;
    
    
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
    component = new ofxDatGuiButton("COLLAPSE ALL");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("EXPAND FOCUSED");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    components.push_back(component);
    
    gui_x += guiCompWidth;
    component = new ofxDatGuiButton("ENABLE/DISABLE");
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 1.0);
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    components.push_back(component);

    
}
//-------------------------------------------------
void TimelinePanel::update(){
    
    if(timeline.getHeight() != _h + guiCompHeight){
        _h = timeline.getHeight() + guiCompHeight;
        for(int i=0; i<components.size(); i++){
            int gui_y = _y + timeline.getHeight();
            components[i]->setPosition(components[i]->getX(), gui_y);
        }
    }
     
    for(int i=0; i<components.size(); i++) components[i]->update();

}
//-------------------------------------------------
void TimelinePanel::draw(){
    
    ofPushStyle();
    ofSetColor(getBackgroundColor());
    ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    timeline.draw();
    
    for(int i=0; i<components.size(); i++) components[i]->draw();

}

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
void TimelinePanel::addTrack(string name, trackType type){
    
    timeline.setCurrentPage(0);
    
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
    
    
}
//--------------------------------------------------------------
void TimelinePanel::removeTrack(string name){
    
    timeline.removeTrack(name);
    
}
//--------------------------------------------------------------
void TimelinePanel::collapseAllTracks(){
    
    int h = timeline.getHeight();
    
    
    page->setFocusedTrack(audioTrack);
    page->collapseAllTracks(TRUE); //exclude focused from collapsing
    page->expandFocusedTrack();
    //timeline.setHeight(h);

}
//--------------------------------------------------------------
void TimelinePanel::expandFocusedTrack(){
    
    //page->evenlyDistributeTrackHeights();
    
    //page->setFocusedTrack(audioTrack);
    //page->expandFocusedTrack();
    
//    ofxTLPage* p = timeline.getPage("Page One");
//    
//    if(p->getFocusedTrack() != NULL)
//        cout<<"FOCUSED TRACK: "<< p->getFocusedTrack()->getName() << endl;
//    else
//        cout<<"NULL"<<endl;
//    
    
}
//--------------------------------------------------------------
#pragma mark - gui listeners
//--------------------------------------------------------------

void TimelinePanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getLabel() << "::" << e.enabled << endl;
    
    string label = e.target->getLabel();
    if(label ==  "ADD"){
        
        addTrack(currentTrackName, currentTrackType);
    
    }else if (label == "REMOVE"){
    
        removeTrack(currentTrackName);
    
    }else if (label == "COLLAPSE ALL"){
        
        collapseAllTracks();
        
    }else if (label == "EXPAND FOCUSED"){
        
        expandFocusedTrack();
        
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.child << endl;
    
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
    cout << "onButtonEvent: " << e.text << endl;
    currentTrackName = e.text;
}


