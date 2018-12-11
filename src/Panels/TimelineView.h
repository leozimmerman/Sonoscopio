//
//  TimelineView.hpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#pragma once

#include "ofMain.h"
#include "View.h"

#include "Macros.h"
#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"
#include "Settings.h"

#define PAGE_AUDIO_NAME "PageAudio"
#define PAGE_TRACKS_NAME "PageTracks"

#define CURVES_STRING "Curves"
#define BANGS_STRING "Bangs"
#define SWITCHES_STRING "Switches"
#define NOTES_STRING "Notes"


enum trackType {
    CURVES,
    BANGS,
    SWITCHES,
    NOTES
};




class TimelineView : public View {
public:
    
    void setup(int x, int y, int w, int h) override;
    void update();
    void draw() override;
    
    void resize(int x, int y, int width, int height) override;
    void updateHeight();
    
    void setupTimeline();
    void keyPressed(int key);
    
    void saveTracksDataToFolder();
    void loadStateIntoSettings(TimelinePanelSettings* settings);
    void setStateFromSettings(TimelinePanelSettings& settings);
    
    void setVisibleTracks(vector<TLTrackSetting> tracks);
    void createNewTrack(string name, string type);
    void removeTrack(string name);
    
    void setFrameRate(int fps);
    void addMarker();
    
    void clearMarkers();
    void toggleShowTracks();
    void toggleEnableDisableFocusedTrack();
    void addKeyframeInFocusedTrack();
    
    void openAudioFile(string filename);
    bool isSoundLoaded(){
        return audioTrack->isSoundLoaded();
    }

    void togglePlay(){ timeline.togglePlay(); }
    void stop(){ timeline.stop(); }
    void rewind(){ timeline.setCurrentTimeToInPoint(); }
    
    void setVolume(float volume){
        if (audioTrack != NULL) {
            audioTrack->setVolume(volume);
        }
    }
    void setLoopType(ofLoopType newType){timeline.setLoopType(newType);}
    void setShowBPMGrid(bool enableGrid){timeline.setShowBPMGrid(enableGrid);}
    void enableSnapToBPM(bool enableSnap){timeline.enableSnapToBPM(enableSnap);}
    void setFrameBased(bool frameBased){timeline.setFrameBased(frameBased);;}
    void setNewBPM(float bpm){timeline.setNewBPM(bpm);}
    void setInPointAtPlayhead(){timeline.setInPointAtPlayhead();}
    void setOutPointAtPlayhead(){timeline.setOutPointAtPlayhead();}
    
    bool existsTrackWithName(string name);
    
    std::map<string, float> getTracksValues();
    
    void bangFired(ofxTLBangEventArgs& args);
    ofxTimeline timeline;
    ofxTLAudioTrack* audioTrack;
   
    vector<TLTrackSetting>& getVisibleTracksRef(){return visibleTracks;}
    vector<TLTrackSetting>& getExistingTracksRef(){return allExistingTracks;}
    
private:
    void createTracksFromTrackSettings(vector<TLTrackSetting> tracks);
    void addExistingTrack(string name, string type);
    void addTrackToTimeline(string name, trackType type);
    void addTrackToTimelineWithStringType(string name, string stringType);
    void loadTracksDataFromFolder();
    void addMarkerAtTime(float millis);
    
    void expandFocusedTrack();
    void hideTracks();
    string typeToString(trackType type);
    
    vector<TLTrackSetting> visibleTracks;
    vector<TLTrackSetting> allExistingTracks;
    
    bool _isThereBang = false;
    ofxTLTrack* _bangedTrack;
    ofColor waveformCol;
    bool _footerIsDragging;
    vector<float> _markers;
    int _frameRate;
    
};
