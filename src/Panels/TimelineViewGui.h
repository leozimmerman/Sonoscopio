//
//  GuiView.hpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#pragma once

#include "GuiView.h"
#include "TimelineView.h"
#include "Settings.h"
#include "TimelineTracksModal.h"
#include "AddTrackModal.h"
#include "RemoveTrackModal.h"

#define ADD_TRACK_LABEL "ADD TRACK"
#define REMOVE_TRACK_LABEL "REMOVE TRACK"
#define SHOW_TRACKS_LABEL "SHOW/HIDE TRACKS"
#define VISIBLE_TRACKS_LABEL "VISIBLE TRACKS"

#define ADD_MARKER_LABEL "ADD MARKER"
#define CLEAR_MARKERS_LABEL "CLEAR MARKERS"
#define BPM_LABEL "BPM"
#define BPM_GRID_LABEL "BPM GRID"
#define SNAP_LABEL "SNAP"
#define FRAMEBASED_LABEL "FRAME BASED"

#define PLAY_STOP_LABEL "> / ||"
#define REWIND_LABEL "|<"
#define VOLUME_LABEL "VOLUME"
#define LOOP_LABEL "LOOP ON-OFF"
#define SET_IN_LABEL "SET IN"
#define SET_OUT_LABEL "SET OUT"

class TimelineViewGui : public GuiView {
public:
    void setup(int x, int y, int w, int h, TimelineView* tv_ptr);
    
    void createComponents() override;
    void adjustComponentsSize() override;
    
    bool getFocused() override;
    
    void loadStateIntoSettings(TimelinePanelSettings* settings);
    void setStateFromSettings(TimelinePanelSettings& settings);
    
private:
    void setupMenus();
    void showVisibleTracksMenu();
    void showAddTrackMenu();
    void showRemoveTrackMenu();
    bool checkForExistingTracks();
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    ofxDatGuiTextInput* gBpm;
    ofxDatGuiSlider* gVolumeSlider;
    ofxDatGuiToggle* gLoopToggle;
    ofxDatGuiToggle* gBpmGridToggle;
    ofxDatGuiToggle* gSnapToggle;
    ofxDatGuiToggle* gFramebasedToggle;
    
    TimelineView* timelineViewPtr;
    shared_ptr<TimelineTracksModal> visibleTracksModal;
    shared_ptr<AddTrackModal> addTracksModal;
    shared_ptr<RemoveTrackModal> removeTracksModal;
    
    string currentTrackName;
    TrackType currentTrackType;
    
};
