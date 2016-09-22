
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
    
    _frameRate = FRAME_RATE;
    
    tMainApp = dynamic_cast<ofApp*>(appPtr);
    
    _bckgColor = ofColor(40);

    waveformCol.set(120);
    
    currentTrackType = CURVES;
    currentTrackName = "";
    
    //-------------------------
    ofxTimeline::removeCocoaMenusFromGlut("Audio Waveform Example");
    timeline.setWorkingFolder(TIMELINE_SETTINGS_DIR);
    
    timeline.setup(PAGE_AUDIO_NAME);///<--tweaked SETUP
    
    timeline.setFrameRate(_frameRate);
    timeline.setAutosave(false);
    timeline.setOffset(ofVec2f(_x, _y));
    
    timeline.setWidth(_w);
    timeline.setHeight(_h);
    
    timeline.setLoopType(OF_LOOP_NONE);
    timeline.setBPM(120.f);
    timeline.setShowBPMGrid(false);

    timeline.addAudioTrack("Audio","audio_files/mix-stereo.wav");//3chans
    
    //this means that calls to play/stop etc will be  routed to the waveform and that timing will be 100% accurate
    timeline.setTimecontrolTrack("Audio");
    timeline.setDurationInSeconds(timeline.getAudioTrack("Audio")->getDuration());
    
    audioTrack = timeline.getAudioTrack("Audio");
    
    timeline.addPage(PAGE_TRACKS_NAME);
    addTrack("DEFAULT", CURVES);

    timeline.setCurrentPage(PAGE_AUDIO_NAME);
    timeline.setShowPageTabs(false); //->modify if more pages are needed
    timeline.setFootersHidden(false);
    
    //timeline.resetInOutTrack();//for using in_out xml file
    timeline.setInOutRange(ofRange(0.0, 1.0));//always start with in/out at the sides, ignores xml file

    
    
    
    
    ofAddListener(timeline.events().bangFired, this, &TimelinePanel::bangFired);
    
    
    //--------------------------
    _guiCompHeight = TL_GUI_COMP_HEIGHT;
    bordCol = ofColor::grey;
    bordWidth = 1;
    setupGui();
    
    
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
    
    drawBackground();
    
    TS_START("waveforms");
    if(timeline.getCurrentPageName() == PAGE_TRACKS_NAME){
        audioTrack->drawWaveforms();
    }
    TS_STOP("waveforms");
    
    //draw gui------------------
    TS_START("gui");
    for(int i=0; i<components.size(); i++){
        components[i]->draw();
    }
    TS_STOP("gui");
    
    //draw timeline------------
    TS_START("timeline");
    timeline.draw(false);//without ticker timeMarks
    TS_STOP("timeline");

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
#pragma mark - Settings
//--------------------------------------------------------------
void TimelinePanel::loadSettings(string rootDir){
   
    //----------------------------------
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
            }else if(trackType=="Bangs"){
                addTrack(trackName, BANGS);
            }else if(trackType=="Switches"){
                addTrack(trackName, SWITCHES);
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
    
    //----------------------------------
    //-:Save Timeline Settings
    timeline.saveTracksToFolder(rootDir + TIMELINE_SETTINGS_DIR);
    

    
    //----------------------------------
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
    
    //----------------------------------
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
void TimelinePanel::startStopPlaying(){
    if(timeline.getIsPlaying()){
        timeline.stop();
    }else{
        timeline.play();
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
void TimelinePanel::toggleShowTracks(){
    
    
    if(timeline.getCurrentPageName() == PAGE_AUDIO_NAME){

        timeline.setCurrentPage(PAGE_TRACKS_NAME);
        timeline.setFootersHidden(TRUE);
        adjustTracksHeight();
        
    }else if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME) {
    
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        timeline.setFootersHidden(false);
        adjustTracksHeight();
        
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::adjustTracksHeight(){
    
    if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME){
        
        int audioH = timeline.getPage(PAGE_AUDIO_NAME)->getComputedHeight();
        int tracksNum = timeline.getPage(PAGE_TRACKS_NAME)->getTracksNum();
        
        timeline.setHeight(audioH + 66 - 17.68*tracksNum);
        
        //cout<<"tracksNum = "<<tracksNum<<endl;
        //cout<<"audioH = "<<audioH<<endl;
        //cout<<"setHeight = "<<audioH + 66 - 17.68*tracksNum<<endl;
        
    }
    
    checkIfHeightChanged();
    
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



//--------------------------------------------------------------
#pragma mark - Info & Value Getters
//--------------------------------------------------------------
string TimelinePanel::getFileInfo(){
    
    string s =
    "duration: " + ofToString(audioTrack->getDuration(), 2) + "sec."
    "\nsample rate: " + ofToString(audioTrack->getSampleRate()) +
    "\nchannels: " + ofToString(audioTrack->getNumChannels()) +
    "\nframes: " + ofToString(timeline.getDurationInFrames()) + " at " + ofToString(_frameRate) + "fps";
    
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
void TimelinePanel::checkIfHeightChanged(){
    
    int tl_h = timeline.getPage(PAGE_AUDIO_NAME)->getDrawRect().height + 66;
    if(tl_h != _h - _guiCompHeight){
        resizeHeight(tl_h);
    }
    
}
//--------------------------------------------------------------
///adjust waveform when zooming in tracks page
void TimelinePanel::checkIfWaveformPreviewChanged(){
    
   
    if(audioTrack->getShouldRecomputePreview() || audioTrack->getViewIsDirty()){
        ofLogVerbose()<<"TimePanel: recomputing audioPreview";
        audioTrack->recomputePreview();
    }
    
}
//--------------------------------------------------------------
void TimelinePanel::resize(int y, int w, int h){
    
    _y = y;
    _w = w;
    _h = h;
    
    //Adjust Timeline
    timeline.setOffset(ofVec2f(_x, _y));
    timeline.setWidth(_w);
    
    timeline.setHeight(_h);
    
    adjustGuiSize(_y, _w, _h);
    
    adjustTracksHeight();
   
}
//--------------------------------------------------------------
void TimelinePanel::resizeHeight(int tl_h){
    
    _h = tl_h + _guiCompHeight;
    
    for(int i=0; i<components.size(); i++){
        int gui_y = _y + tl_h;
        components[i]->setPosition(components[i]->getX(), gui_y);
    }
    
    ofNotifyEvent(heightResizedEvent, _h, this);
}
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
    
    vector<string> options = {"curves", "bangs", "switches"};
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
    int gui_y = _y + timeline.getHeight();
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


