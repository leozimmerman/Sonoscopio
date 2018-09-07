//
//  TimelineView.cpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#include "TimelineView.h"

void TimelineView::setup(int x, int y, int w, int h, string audiofile) {
    View::setup(x, y, w, h);
    setBackgroundColor(ofColor(40));
    waveformCol.set(120);
    _frameRate = INIT_FPS;
    setupTimeline(audiofile);
    
}

void TimelineView::update(){}
void TimelineView::draw(){
    //TODO:Revisar esta separacion entre timeline y audioWaveform...
    View::draw();//background
    if(timeline.getCurrentPageName() == PAGE_TRACKS_NAME){
        audioTrack->drawWaveforms();
    }
    timeline.draw(false, false);//without ticker timeMarks & hidden mode
}
void TimelineView::resize(int x, int y, int width, int height){
    View::resize(x, y, width, height);
    timeline.setOffset(ofVec2f(_x, _y));
    timeline.setWidth(_w);
    updateHeight();
}

void TimelineView::setupTimeline(string audiofile) {
    
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
    updateHeight();
    
    ofAddListener(timeline.events().bangFired, this, &TimelineView::bangFired);
}

void TimelineView::keyPressed(int key){
    
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    
    switch (key) {
        case 'e':
            expandFocusedTrack();
            break;
        case 'd':
            toggleEnableDisableFocusedTrack();
            break;
        case 'a':
            updateHeight();
            break;
            
        default:
            break;
    }
}

void TimelineView::openAudioFile(string filename){
    timeline.stop();
    timeline.setCurrentTimeSeconds(0.0);
    audioTrack->loadSoundfile(filename);
    timeline.setDurationInSeconds(audioTrack->getDuration());
    
}
//--------------------------------------------------------------
void TimelineView::setFrameRate(int fps){
    
    _frameRate = fps;
    timeline.setFrameRate(_frameRate);
}

//--------------------------------------------------------------
void TimelineView::bangFired(ofxTLBangEventArgs& args){
    //cout << "bang fired!" << args.flag << endl;
    cout << "bang fired!" << endl;
    _isThereBang = TRUE;
    _bangedTrack = args.track;
}

//--------------------------------------------------------------
#pragma mark - Tracks
//--------------------------------------------------------------
void TimelineView::addTrack(string name, trackType type){
    
    string currentPageName = timeline.getCurrentPageName();
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
    
    updateHeight();
    
}
//--------------------------------------------------------------
void TimelineView::removeTrack(string name){
    timeline.removeTrack(name);
    updateHeight();
}
//--------------------------------------------------------------
void TimelineView::hideTracks(){
    if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME) {
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        timeline.setFootersHidden(false);
        updateHeight();
    }
}
//--------------------------------------------------------------
void TimelineView::toggleShowTracks(){
    
    if(timeline.getCurrentPageName() == PAGE_AUDIO_NAME){
        timeline.setCurrentPage(PAGE_TRACKS_NAME);
        //timeline.setFootersHidden(TRUE);
    }else if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME) {
        timeline.setCurrentPage(PAGE_AUDIO_NAME);
        //timeline.setFootersHidden(false);
    }
    updateHeight();
}
//--------------------------------------------------------------
void TimelineView::updateHeight(){
    if (timeline.getCurrentPageName() == PAGE_TRACKS_NAME){
        int tracksNum = timeline.getPage(PAGE_TRACKS_NAME)->getTracksNum();
        timeline.setHeight(_h - 20.0 - 17.68 * (tracksNum-1));
    } else {
        timeline.setHeight(_h - 20);
    }
}
//--------------------------------------------------------------
void TimelineView::expandFocusedTrack(){
    ofxTLTrack* ftrack = timeline.getFocusedTrack();
    if(ftrack != NULL && timeline.getCurrentPageName()==PAGE_TRACKS_NAME){
        timeline.getPage(PAGE_TRACKS_NAME)->expandFocusedTrack();
    }
}
//--------------------------------------------------------------
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
#pragma mark - Info & Value Getters
//--------------------------------------------------------------
string TimelineView::getFileInfo(){
    
    string s =
    "duration: " + ofToString(audioTrack->getDuration(), 2) + "sec."
    " - sample rate: " + ofToString(audioTrack->getSampleRate()) +
    " - channels: " + ofToString(audioTrack->getNumChannels()) +
    " - frames: " + ofToString(timeline.getDurationInFrames()) + " at " + ofToString(_frameRate) + "fps";
    
    return s;
    
}

//--------------------------------------------------------------
std::map<string, float> TimelineView::getTracksValues(){
    
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
void TimelineView::addMarker(){
    timeline.getTicker()->addMarker(timeline.getCurrentTimeMillis());
    _markers.push_back(timeline.getCurrentTimeMillis());
}
//--------------------------------------------------------------
void TimelineView::addMarker(float millis){
    
    float ms =  ofClamp(millis, 0.0, timeline.getDurationInMilliseconds());//clamp just in case...
    timeline.getTicker()->addMarker(ms);
    _markers.push_back(ms);
    
}
//--------------------------------------------------------------
void TimelineView::clearMarkers(){
    timeline.getTicker()->clearMarkers();
    _markers.clear();
}

void TimelineView::checkIfWaveformPreviewChanged(){
    //FIXME: No se esta usando. Revisar que no se ajusta el waveform cuando se muestran los tracks.
    if(audioTrack->getShouldRecomputePreview() || audioTrack->getViewIsDirty()){
        ofLogVerbose()<<"TimePanel: recomputing audioPreview";
        audioTrack->recomputePreview();
    }
}
