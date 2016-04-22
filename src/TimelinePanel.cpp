
#include "TimelinePanel.h"

#include "ofApp.h"

ofApp* tMainApp;

//-------------------------------------------------

void TimelinePanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    tMainApp = dynamic_cast<ofApp*>(appPtr);
    
    setBackgroundColor(ofColor::darkBlue);
    
    //-------------------------
    ofxTimeline::removeCocoaMenusFromGlut("Audio Waveform Example");
    
    timeline.setup();
    timeline.setOffset(ofVec2f(_x, _y));
    timeline.setWidth(_w);
    timeline.setHeight(_h);
    timeline.setLoopType(OF_LOOP_NORMAL);
    timeline.addAudioTrack("Audio","audio_files/rock.mp3");
    //this means that calls to play/stop etc will be  routed to the waveform and that timing will be 100% accurate
    timeline.setTimecontrolTrack("Audio");
    timeline.setDurationInSeconds(timeline.getAudioTrack("Audio")->getDuration());
    track = timeline.getAudioTrack("Audio");
    
    



    
}
//-------------------------------------------------
void TimelinePanel::update(){

}
//-------------------------------------------------
void TimelinePanel::draw(){
    
    ofPushStyle();
    ofSetColor(getBackgroundColor());
    ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    timeline.draw();
    


}
//--------------------------------------------------------------
void TimelinePanel::openAudioFile(string filename){
    timeline.stop();
    timeline.setCurrentTimeSeconds(0.0);
    track->loadSoundfile(filename);
    timeline.setDurationInSeconds(track->getDuration());
    
   
    
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
    "duration: " + ofToString(track->getDuration(), 2) + "sec."
    "\nsample rate: " + ofToString(track->getSampleRate()) +
    "\nchannels: " + ofToString(track->getNumChannels()) +
    "\nbuffer size: " + ofToString(track->getBufferSize());
    
    return s;
    
}



