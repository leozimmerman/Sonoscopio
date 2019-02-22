//
//  TimelineView.cpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#include "ofApp.h"
#include "TimelineView.h"
#include "FileManager.h"

#pragma mark - Core
void TimelineView::setup(int x, int y, int w, int h) {
    View::setup(x, y, w, h);
    setBackgroundColor(ofColor(40));
    waveformCol.set(120);
    _frameRate = INIT_FPS; //TODO: Fix
    setupTimeline();
}

void TimelineView::update(){}
void TimelineView::draw(){
    View::draw();
    if (audioTrack->isSoundLoaded()){
        if(timeline.getCurrentPageName() == PAGE_TRACKS_NAME){
            audioTrack->drawWaveforms();
        }
        timeline.draw(true, false);
    }
}

void TimelineView::resize(int x, int y, int width, int height){
    View::resize(x, y, width, height);
    timeline.setOffset(ofVec2f(_x, _y));
    timeline.setWidth(_w);
    updateHeight();
}

void TimelineView::setClicksEnabled(bool enabled){
    View::setClicksEnabled(enabled);
    enabled ? timeline.enableEvents() : timeline.disableEvents();
}

bool TimelineView::keyPressed(int key){
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    
    // c/cmd
    if (ofGetModifierShortcutKeyPressed()) {
        switch (key) {
            case 'e':
                expandFocusedTrack();
                return true;
            case 'd':
                toggleEnableDisableFocusedTrack();
                return true;
            case 'a':
                updateHeight();
                return true;
            case 'k':
                addKeyframeInFocusedTrack();
                return true;
          
        }
    }
    
    switch (key) {
 
        case 'w':
            rewind();
            return true;
        case 'm':
            addMarker();
            return true;
        
        default:
            break;
    }
    return false;
}

#pragma mark - Gral
void TimelineView::setupTimeline() {
    
    ofxTimeline::removeCocoaMenusFromGlut("Audio Waveform Example");
    //timeline.setWorkingFolder(FileManager::getInstance().getTimelineFolderPath());
    
    timeline.setup(PAGE_AUDIO_NAME);///<--tweaked SETUP
    
    timeline.setFrameRate(_frameRate);
    timeline.setAutosave(false);
    timeline.setOffset(ofVec2f(_x, _y));
    
    
    timeline.setLoopType(OF_LOOP_NONE);
    timeline.setBPM(TL_DEFAULT_INIT_BPM);
    timeline.setShowBPMGrid(false);
    
    timeline.addAudioTrack("Audio", ""); /// Add empty audio Track?
    
    //this means that calls to play/stop etc will be  routed to the waveform anofd that timing will be 100% accurate
    timeline.setTimecontrolTrack("Audio");
    timeline.setDurationInSeconds(timeline.getAudioTrack("Audio")->getDuration());
    
    audioTrack = timeline.getAudioTrack("Audio");
    
    timeline.addPage(PAGE_TRACKS_NAME);
    timeline.setCurrentPage(PAGE_TRACKS_NAME);
    timeline.setShowPageTabs(false); //->modify if more pages are needed
    timeline.setFootersHidden(true);
    
    //timeline.resetInOutTrack();//for using in_out xml file
    timeline.setInOutRange(ofRange(0.0, 1.0));//always start with in/out at the sides, ignores xml file
    timeline.setWidth(_w);
    updateHeight();
    
}


void TimelineView::openAudioFile(string filename){
    timeline.stop();
    timeline.setCurrentTimeSeconds(0.0);
    audioTrack->loadSoundfile(filename);
    timeline.setDurationInSeconds(audioTrack->getDuration());
}

void TimelineView::setFrameRate(int fps){
    _frameRate = fps;
    timeline.setFrameRate(_frameRate);
}

void TimelineView::updateHeight(){
    if (timeline.getCurrentPageName() == PAGE_AUDIO_NAME){
        timeline.setHeight(_h - 20);
    } else {
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        timeline.setHeight(_h - 20);
        
        timeline.setCurrentPage(PAGE_TRACKS_NAME);
        int tracksNum = timeline.getPage(PAGE_TRACKS_NAME)->getTracksNum();
        if (tracksNum == 0){tracksNum = 1;}
        timeline.setHeight(_h - 20.0 - 17.68 * (tracksNum-1));
    }
}


#pragma mark - Tracks Add/Remove

void TimelineView::createNewTrack(string name, string type ){
    if (existsTrackWithName(name)){
        showErrorMessage(TRACK_ALREADY_EXISTS);
        return;
    }
    addTrack(name, type);
    loadTracksDataFromFolder();
}

void TimelineView::removeTrack(string name){
    timeline.removeTrack(name);
    removeExistingTrackWithName(name);
    hideTracksIfNeeded();
    updateHeight();
}

void TimelineView::addTrack(string name, string type){
    TLTrackSetting ts;
    ts.name = name;
    ts.type = type;
    allExistingTracks.push_back(ts);
    visibleTracks.push_back(ts);
    addTrackToTimelineWithStringType(name, type);
}

void TimelineView::addTrackToTimeline(string name, TrackType type){
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
    updateHeight();
}

void TimelineView::addTrackToTimelineWithStringType(string name, string stringType){
    if(stringType==CURVES_STRING){
        addTrackToTimeline(name, CURVES);
    } else if(stringType==BANGS_STRING){
        addTrackToTimeline(name, BANGS);
    } else if(stringType==SWITCHES_STRING){
        addTrackToTimeline(name, SWITCHES);
    } else if(stringType==NOTES_STRING){
        addTrackToTimeline(name, NOTES);
    } else {
        ofLogError() << "Wrong track Type";
    }
}

bool TimelineView::existsTrackWithName(string name){
    for (auto t : allExistingTracks){
        if (t.name == name){
            return true;
        }
    }
    return false;
}

string TimelineView::typeToString(TrackType type){
    switch (type) {
        case CURVES:
            return CURVES_STRING;
            break;
        case BANGS:
            return BANGS_STRING;
            break;
        case SWITCHES:
            return SWITCHES_STRING;
            break;
        case NOTES:
            return NOTES_STRING;
            break;
        default:
            break;
    }
}

void TimelineView::removeExistingTrackWithName(string name){
    for (int i=0; i<allExistingTracks.size(); i++){
        if (allExistingTracks[i].name == name){
            allExistingTracks.erase(allExistingTracks.begin()+i);
            break;
        }
    }
    for (int i=0; i<visibleTracks.size(); i++){
        if (visibleTracks[i].name == name){
            visibleTracks.erase(visibleTracks.begin()+i);
            break;
        }
    }
}

void TimelineView::createTracksFromTrackSettings(vector<TLTrackSetting> tracks){
    auto tracksPage = timeline.getPage(PAGE_TRACKS_NAME);
    tracksPage->removeAllTracks();
    for (auto t : tracks){
        if(tracksPage->getTrack(t.name)==NULL){
            addTrackToTimelineWithStringType(t.name, t.type);
        }
    }
}

#pragma mark - Visible tracks

void TimelineView::setVisibleTracks(vector<TLTrackSetting> tracks){
    visibleTracks = tracks;
    
    auto tracksPage = timeline.getPage(PAGE_TRACKS_NAME);
    saveTracksDataToFolder();
    tracksPage->removeAllTracks();
    createTracksFromTrackSettings(visibleTracks);
    loadTracksDataFromFolder();
    hideTracksIfNeeded();
}

#pragma mark - Tracks display

void TimelineView::hideTracks(){
    if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME) {
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        timeline.setFootersHidden(false);
        updateHeight();
    }
}

void  TimelineView::hideTracksIfNeeded() {
    setTracksHidden(visibleTracks.size() == 0);
}

void TimelineView::toggleShowTracks(){
    setTracksHidden(!getTracksHidden());
    updateHeight();
}

void TimelineView::setTracksHidden(bool hide){
    auto pageName = hide ? PAGE_AUDIO_NAME : PAGE_TRACKS_NAME;
    timeline.setCurrentPage(pageName);
}

bool TimelineView::getTracksHidden(){
    return timeline.getCurrentPageName() == PAGE_AUDIO_NAME;
}

void TimelineView::expandFocusedTrack(){
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    if(ftrack != NULL && timeline.getCurrentPageName()==PAGE_TRACKS_NAME){
        timeline.getPage(PAGE_TRACKS_NAME)->expandFocusedTrack();
    }
}

void TimelineView::toggleEnableDisableFocusedTrack(){
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    if(ftrack != NULL && timeline.getCurrentPageName()==PAGE_TRACKS_NAME){
        ftrack->isEnabled() ? ftrack->disable() : ftrack->enable();
    }
}

void TimelineView::addKeyframeInFocusedTrack(){
    ofxTLKeyframes* ftrack = (ofxTLKeyframes*) timeline.getFocusedTrack();
    if (ftrack != NULL) {
        ftrack->addKeyframe();
    }
}
#pragma mark - Markers

void TimelineView::addMarker(){
    timeline.getTicker()->addMarker(timeline.getCurrentTimeMillis());
    _markers.push_back(timeline.getCurrentTimeMillis());
}

void TimelineView::addMarkerAtTime(float millis){
    float ms =  ofClamp(millis, 0.0, timeline.getDurationInMilliseconds());//clamp just in case...
    timeline.getTicker()->addMarker(ms);
    _markers.push_back(ms);
}

void TimelineView::clearMarkers(){
    timeline.getTicker()->clearMarkers();
    _markers.clear();
}

#pragma mark - Value Getters

std::map<string, float> TimelineView::getTracksValues(){
    std::map<string, float> values;
    ofxTLPage* trPage = timeline.getPage(PAGE_TRACKS_NAME);
    
    for (int i=0; i<trPage->getTracksNum(); i++){
        for(ofxTLTrack* track : trPage->getTracks()){
            string name = track->getName();
            string key = name;
            float value;
            if(track->getTrackType()==CURVES_STRING){
                value = timeline.getValue(name);
            }else if(track->getTrackType()==SWITCHES_STRING){
                value = timeline.isSwitchOn(name);
            }else if(track->getTrackType()==BANGS_STRING){
                value = timeline.isBang(name);
            }else if(track->getTrackType()==NOTES_STRING){
                value = timeline.getNote(name);
            }
            values[key]= value;
        }
    }
    return values;
}

#pragma mark - Settings

void TimelineView::loadStateIntoSettings(TimelinePanelSettings* settings){
    settings->tracks.clear();
    settings->tracks.shrink_to_fit();
    for (auto track : allExistingTracks) {
        TLTrackSetting ts;
        ts.name = track.name;
        ts.type = track.type;
        settings->tracks.push_back(ts);
    }
    settings->markers.clear();
    for (auto marker : _markers){
        settings->markers.push_back(marker);
    }
}

void TimelineView::setStateFromSettings(TimelinePanelSettings& settings){
    clearMarkers();
    for (auto m : settings.markers){
        addMarkerAtTime(m);
    }
    createTracksFromTrackSettings(settings.tracks);
    allExistingTracks = settings.tracks;
    visibleTracks = allExistingTracks;
    loadTracksDataFromFolder();
    hideTracksIfNeeded();
    updateHeight();
}

void TimelineView::loadTracksDataFromFolder(){
    string folderPath = FileManager::getInstance().getTimelineFolderPath();
    timeline.loadTracksFromFolder(folderPath);
}
void TimelineView::saveTracksDataToFolder(){
    string folderPath = FileManager::getInstance().getTimelineFolderPath();
    timeline.saveTracksToFolder(folderPath);
}

#pragma mark - Modal Messages
const string getErrorMessage(TLViewError error){
    switch (error) {
        case TRACK_ALREADY_EXISTS: return "A track with that name already exists";
        case NONE_EXISTING_TRACKS: return "You need to add at least one track first";
    }
}

void TimelineView::showErrorMessage(TLViewError error){
    string message = getErrorMessage(error);
    ofNotifyEvent(ofApp::errorEvent, message);
}





