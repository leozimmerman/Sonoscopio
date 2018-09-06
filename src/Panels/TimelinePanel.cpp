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

#include "TimelinePanel.h"

#include "ofApp.h"

ofxDatGuiDropdown* dropdown;

#pragma mark - Core funcs

//-------------------------------------------------

void TimelinePanel::setup(int x, int y, int width, int height, string audiofile){
    
    BasePanel::setup(x, y, width, height);
    setBackgroundColor(ofColor(40));
    _frameRate = INIT_FPS;
    
    waveformCol.set(120);
    
    currentTrackType = CURVES;
    currentTrackName = "";
    
    _guiCompHeight = TL_GUI_COMP_HEIGHT;
    bordCol = ofColor::grey;
    bordWidth = 1;
    setupGui();
    
    setupTimeline(audiofile);
    
}
void TimelinePanel::setupTimeline(string audiofile) {
    
    ofxTimeline::removeCocoaMenusFromGlut("Audio Waveform Example");
    timeline.setWorkingFolder(TIMELINE_SETTINGS_DIR);
    
    timeline.setup(PAGE_AUDIO_NAME);///<--tweaked SETUP
    
    timeline.setFrameRate(_frameRate);
    timeline.setAutosave(false);
    timeline.setOffset(ofVec2f(_x, _y));

    
    timeline.setLoopType(OF_LOOP_NONE);
    timeline.setBPM(120.f);
    timeline.setShowBPMGrid(false);
    
    timeline.addAudioTrack("Audio", audiofile);
    
    //this means that calls to play/stop etc will be  routed to the waveform anofd that timing will be 100% accurate
    timeline.setTimecontrolTrack("Audio");
    timeline.setDurationInSeconds(timeline.getAudioTrack("Audio")->getDuration());
    
    audioTrack = timeline.getAudioTrack("Audio");
    
    timeline.addPage(PAGE_TRACKS_NAME);
    addTrack("DEFAULT", CURVES);
    
    timeline.setCurrentPage(PAGE_AUDIO_NAME);
    timeline.setShowPageTabs(false); //->modify if more pages are needed
    timeline.setFootersHidden(true);
    
    //timeline.resetInOutTrack();//for using in_out xml file
    timeline.setInOutRange(ofRange(0.0, 1.0));//always start with in/out at the sides, ignores xml file
    timeline.setWidth(_w);
    timeline.setHeight(_h - _guiCompHeight - 20);
    
    ofAddListener(timeline.events().bangFired, this, &TimelinePanel::bangFired);
}

//-------------------------------------------------
void TimelinePanel::update(){
    
    _footerIsDragging = timeline.getPage(timeline.getCurrentPageName())->getFooterIsDragging();
    
    if(!_footerIsDragging){ //avoid hittin buttons when dragging
        for(int i=0; i<components.size(); i++){
            components[i]->update();
        }
    }
    
}
//-------------------------------------------------
void TimelinePanel::draw(){
    if (_isHidden){ return; }
    View::draw();
    
    TS_START("waveforms");
    if(timeline.getCurrentPageName() == PAGE_TRACKS_NAME && !_isHidden){
        audioTrack->drawWaveforms();
    }
    TS_STOP("waveforms");
    
    TS_START("timeline");
    timeline.draw(false, _isHidden);//without ticker timeMarks & hidden mode
    TS_STOP("timeline");
    
    
    TS_START("gui");
    
    for(int i=0; i<components.size(); i++){
        components[i]->draw();
    }

    TS_STOP("gui");

}
//--------------------------------------------------------------
void TimelinePanel::keyPressed(int key){
    
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    
    switch (key) {
        case 'e':
            expandFocusedTrack();
            break;
        case 'd':
            toggleEnableDisableFocusedTrack();
            break;
        case 'a':
            adjustTracksHeight();
            break;
            
            
        default:
            break;
    }
}

//--------------------------------------------------------------
bool TimelinePanel::getFocused(){
    if (gTrackName->getFocused()) {
        return true;
    } else {
        return false;
    }
    
    
}
void TimelinePanel::exit(){}
//--------------------------------------------------------------
#pragma mark - Settings
//--------------------------------------------------------------
void TimelinePanel::loadSettings(string rootDir){
   
    
    //-:Load Xml file
    
    string filenamePanel = rootDir + TIMELINE_SETTINGS_DIR "/timePanel_settings.xml";
    ofxXmlSettings xml;
    
    if( xml.loadFile(filenamePanel) ){
        ofLogVerbose()<<"TimePanel: "<< filenamePanel <<" loaded.";
    }else{
        ofLogError()<< "TimePanel: unable to load " << filenamePanel ;
        return;
    }
    
    //----------------------------------
    //-:Add markers from loaded settings.
    
    clearMarkers();
    
    int markersNum = xml.getValue("PANEL:MARKERS:MARKERS-NUM", 0);
    for (int i=0; i<markersNum; i++){
        float markerTimeMillis = xml.getValue("PANEL:MARKERS:MARKER-" + ofToString(i), 0.0);
        addMarker(markerTimeMillis);
    }
    //----------------------------------
    //-:Create tracks from loaded settings.
    int tracksNum= xml.getValue("PANEL:TRACKS:TRACKS-NUM", 0);
    for (int i=0; i<tracksNum; i++){
        string trackName = xml.getValue("PANEL:TRACKS:TRACK-" + ofToString(i) +":NAME", "");
        string trackType = xml.getValue("PANEL:TRACKS:TRACK-" + ofToString(i) +":TYPE", "");
        
        auto tracksPage = timeline.getPage(PAGE_TRACKS_NAME);
        
        //If track doesnt exist and its not default -> create it.
        if(trackName != "DEFAULT" &&
           tracksPage->getTrack(trackName)==NULL){
           
            if(trackType=="Curves"){
                addTrack(trackName, CURVES);
            } else if(trackType=="Bangs"){
                addTrack(trackName, BANGS);
            } else if(trackType=="Switches"){
                addTrack(trackName, SWITCHES);
            } else if(trackType=="Notes"){
                addTrack(trackName, NOTES);
            }
            
        }
    }
    //----------------------------------
    //-:Load Timeline Settings
    timeline.loadTracksFromFolder(rootDir + TIMELINE_SETTINGS_DIR"/");
    adjustTracksHeight();
    
    //----------------------------------
    //-:Log
    
    ofLogVerbose()<<"TimePanel: settings LOADED";
    ofLogVerbose()<<"-Tracks Loaded: "<< tracksNum;
    ofLogVerbose()<<"-Markers Loaded: "<< markersNum;
    

}
//--------------------------------------------------------------
void TimelinePanel::saveSettings(string rootDir){
    
    //-:Save Timeline Settings
    timeline.saveTracksToFolder(rootDir + TIMELINE_SETTINGS_DIR);
    
    //-:Save Tracks Names and Types
    
    auto tracksPage = timeline.getPage(PAGE_TRACKS_NAME);

    string filenamePanel = rootDir + TIMELINE_SETTINGS_DIR "/timePanel_settings.xml";
    ofxXmlSettings savedSettings;
    
    int tracksNum = tracksPage->getTracks().size();
    
    savedSettings.addTag("PANEL");
    savedSettings.pushTag("PANEL");
    savedSettings.addTag("TRACKS");
    savedSettings.pushTag("TRACKS");
    savedSettings.addValue("TRACKS-NUM", tracksNum);
    
    for (int i=0; i<tracksNum; i++){
        savedSettings.addTag("TRACK-"+ofToString(i));
        savedSettings.pushTag("TRACK-"+ofToString(i));
        savedSettings.addValue("NAME", tracksPage->getTracks()[i]->getName());
        savedSettings.addValue("TYPE", tracksPage->getTracks()[i]->getTrackType());
        savedSettings.popTag();
    }
    savedSettings.popTag();
    

    //-:Save Markers
    
    savedSettings.addTag("MARKERS");
    savedSettings.pushTag("MARKERS");
    
    savedSettings.addValue("MARKERS-NUM", (int)_markers.size());
    for (int i=0; i<_markers.size(); i++){
        savedSettings.addValue("MARKER-"+ofToString(i), _markers[i]);
    }
    
    savedSettings.saveFile(filenamePanel);
    
    //----------------------------------
    //-:Log
    
    ofLogVerbose()<<"TimePanel: settings SAVED";
    ofLogVerbose()<<"-Tracks Saved: "<< tracksNum;
    for (auto track : tracksPage->getTracks()){
        ofLogVerbose()<<"--Name: "<<track->getName() << "- Type: "<< track->getTrackType();
    }
    ofLogVerbose()<<"-Markers Saved: "<< _markers.size();
    
    

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
void TimelinePanel::setFrameRate(int fps){
    
    _frameRate = fps;
    timeline.setFrameRate(_frameRate);
}

//--------------------------------------------------------------
void TimelinePanel::bangFired(ofxTLBangEventArgs& args){
    //cout << "bang fired!" << args.flag << endl;
    cout << "bang fired!" << endl;
    _isThereBang = TRUE;
    _bangedTrack = args.track;
}

//--------------------------------------------------------------
#pragma mark - Tracks
//--------------------------------------------------------------
void TimelinePanel::addTrack(string name, trackType type){
    
    string currentPageName = timeline.getCurrentPageName();
    
    //-
    
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
            break;
        case NOTES:
            timeline.addNotes(name);
            break;
            
        default:
            break;
    }
    
    //-If track was added from audio page, go back  to it.
    
    if(currentPageName == PAGE_AUDIO_NAME){
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
    }
    
    adjustTracksHeight();
    
    
}
//--------------------------------------------------------------
void TimelinePanel::removeTrack(string name){
    
    timeline.removeTrack(name);
    adjustTracksHeight();
    
}
//--------------------------------------------------------------
void TimelinePanel::hideTracks(){
    if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME) {
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        timeline.setFootersHidden(false);
        adjustTracksHeight();
    }
}
//--------------------------------------------------------------
void TimelinePanel::toggleShowTracks(){
    
    if(timeline.getCurrentPageName() == PAGE_AUDIO_NAME){
        timeline.setCurrentPage(PAGE_TRACKS_NAME);
        //timeline.setFootersHidden(TRUE);
        adjustTracksHeight();
    }else if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME) {
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        //timeline.setFootersHidden(false);
        adjustTracksHeight();
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::adjustTracksHeight(){

    if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME){
        int audioH = timeline.getPage(PAGE_AUDIO_NAME)->getComputedHeight();
        int tracksNum = timeline.getPage(PAGE_TRACKS_NAME)->getTracksNum();
        //timeline.setHeight(audioH + _strangeMargin1 - 17.68*tracksNum);
        
    }
    //timeline.setHeight(_h - _guiCompHeight - 50);
    //checkIfHeightChanged();

}
//--------------------------------------------------------------
void TimelinePanel::expandFocusedTrack(){
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    if(ftrack != NULL && timeline.getCurrentPageName()==PAGE_TRACKS_NAME){
        timeline.getPage(PAGE_TRACKS_NAME)->expandFocusedTrack();
    }
}
//--------------------------------------------------------------
void TimelinePanel::toggleEnableDisableFocusedTrack(){
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    if(ftrack != NULL && timeline.getCurrentPageName()==PAGE_TRACKS_NAME){
        ftrack->isEnabled() ? ftrack->disable() : ftrack->enable();
    }
}
void TimelinePanel::addKeyframeInFocusedTrack(){
    ofxTLKeyframes* ftrack = (ofxTLKeyframes*) timeline.getFocusedTrack();
    if (ftrack != NULL) {
        ftrack->addKeyframe();
    }
}


//--------------------------------------------------------------
#pragma mark - Info & Value Getters
//--------------------------------------------------------------
string TimelinePanel::getFileInfo(){
    
    string s =
    "duration: " + ofToString(audioTrack->getDuration(), 2) + "sec."
    " - sample rate: " + ofToString(audioTrack->getSampleRate()) +
    " - channels: " + ofToString(audioTrack->getNumChannels()) +
    " - frames: " + ofToString(timeline.getDurationInFrames()) + " at " + ofToString(_frameRate) + "fps";
    
    return s;
    
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
            }else if(track->getTrackType()=="Notes"){
                value = timeline.getNote(name);
            }
            //add key & value
            values[key]= value;
        }
        
    }
    
    return values;
}
//--------------------------------------------------------------
#pragma mark - Size
//--------------------------------------------------------------
//void TimelinePanel::checkIfHeightChanged(){
//    //FIXME: Esto vuela??
//    int tl_h = timeline.getPage(PAGE_AUDIO_NAME)->getDrawRect().height + _strangeMargin1;
//
//    if(tl_h != _h - _guiCompHeight){
//     ///   resizeHeight(tl_h);
//    }
//
//}
//--------------------------------------------------------------
///adjust waveform when zooming in tracks page
void TimelinePanel::checkIfWaveformPreviewChanged(){
    
    if(audioTrack->getShouldRecomputePreview() || audioTrack->getViewIsDirty()){
        ofLogVerbose()<<"TimePanel: recomputing audioPreview";
        audioTrack->recomputePreview();
    }
}
//--------------------------------------------------------------
void TimelinePanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    
    //Adjust Timeline
    timeline.setOffset(ofVec2f(_x, _y));
    timeline.setWidth(_w);
    adjustGuiSize(_y, _w, _h);
    timeline.setHeight(_h - _guiCompHeight - 20);

    ///adjustTracksHeight();
   
}
//--------------------------------------------------------------
//void TimelinePanel::resizeHeight(int tl_h){
//
//    _h = tl_h + _guiCompHeight;
//
//    for(int i=0; i<components.size(); i++){
//        int gui_y = _y + tl_h;
//        components[i]->setPosition(components[i]->getX(), gui_y);
//    }
//
//    ofNotifyEvent(heightResizedEvent, _h, this);
//}
//--------------------------------------------------------------
//void TimelinePanel::setHidden(bool h){
//    
//    _isHidden = h;
//    
//    if (_isHidden) {
//        _guiCompHeight = 1;
//        resize(_y, _w, _strangeMargin1);
//        ofNotifyEvent(heightResizedEvent, _h, this);
//    } else {
//        _guiCompHeight = TL_GUI_COMP_HEIGHT;
//        resize(_y, _w, TIME_PANEL_HEIGHT * ofGetHeight());
//    }
//}
//--------------------------------------------------------------
#pragma mark - Markers
//--------------------------------------------------------------
void TimelinePanel::addMarker(){
    timeline.getTicker()->addMarker(timeline.getCurrentTimeMillis());
    _markers.push_back(timeline.getCurrentTimeMillis());
    
}
//--------------------------------------------------------------
void TimelinePanel::addMarker(float millis){
    
    float ms =  ofClamp(millis, 0.0, timeline.getDurationInMilliseconds());//clamp just in case...
    timeline.getTicker()->addMarker(ms);
    _markers.push_back(ms);
    
}
//--------------------------------------------------------------
void TimelinePanel::clearMarkers(){
    timeline.getTicker()->clearMarkers();
    _markers.clear();
}

//--------------------------------------------------------------
#pragma mark - Gui
//--------------------------------------------------------------
void TimelinePanel::setupGui(){
    
    ofxDatGuiComponent* component;
    
    gTrackName = new ofxDatGuiTextInput("TRACK NAME", "#track name#");
    component = gTrackName;
    
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &TimelinePanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    vector<string> options = {"curves", "bangs", "switches", "notes"};
    dropdown = new ofxDatGuiDropdown("TRACK TYPE", options);
    //dropdown->expand();
    dropdown->onDropdownEvent(this, &TimelinePanel::onDropdownEvent);
    component = dropdown;
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setStripeColor(bordCol);
    components.push_back(dropdown);
    
    
    component = new ofxDatGuiButton("ADD TRACK");
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("REMOVE TRACK");
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SHOW TRACKS");
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("ADJUST TRACKS");
    component->onButtonEvent(this, &TimelinePanel::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    //-:Set components Positions and Widths
    adjustGuiSize(_y, _w, _h);
    
}
//--------------------------------------------------------------
void TimelinePanel::adjustGuiSize(int y, int w, int h){
    
    int guiCompWidth = _w / 7;
    int gui_y = ofGetHeight() - _guiCompHeight;
    int gui_x = _x;
    
    ofxDatGuiComponent* component;
    //TRACK NAME
    component = components[0];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth * 2, 0.5);//width x2
    
    //TRACK TYPE
    gui_x += guiCompWidth*2;
    component = components[1];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //ADD
    gui_x += guiCompWidth;
    component = components[2];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //REMOVE
    gui_x += guiCompWidth;
    component = components[3];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //SHOW TRACKS
    gui_x += guiCompWidth;
    component = components[4];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //ADJUST TRACKS
    gui_x += guiCompWidth;
    component = components[5];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    
}
//--------------------------------------------------------------

void TimelinePanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    //ofLogVerbose() << "onButtonEvent: " << e.target->getLabel() << "::" << e.enabled ;
    
    string label = e.target->getLabel();
    if(label ==  "ADD TRACK"){
        addTrack(currentTrackName, currentTrackType);
    }else if (label == "REMOVE TRACK"){
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
        case 3:
            currentTrackType = NOTES;
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


