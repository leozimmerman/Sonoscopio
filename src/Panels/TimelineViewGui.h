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

#define FRAMEBASED_LABEL "FRAME BASED"
#define PLAY_STOP_LABEL "PLAY / STOP"
#define VOLUME_LABEL "VOLUME"
#define LOOP_LABEL "LOOP ON-OFF"
#define ADD_MARKER_LABEL "ADD MARKER"
#define CLEAR_MARKERS_LABEL "CLEAR MARKERS"

#define BPM_LABEL "BPM"
#define BPM_GRID_LABEL "BPM GRID"
#define SNAP_LABEL "SNAP"
#define SET_IN_LABEL "SET IN"
#define SET_OUT_LABEL "SET OUT"

#define TRACK_NAME_LABEL "TRACK NAME"
#define TRACK_TYPE_LABEL "TRACK TYPE"
#define ADD_TRACK_LABEL "ADD TRACK"
#define REMOVE_TRACK_LABEL "REMOVE TRACK"
#define SHOW_TRACKS_LABEL "SHOW TRACKS"
#define ADJUST_TRACKS_LABEL "ADJUST TRACKS"

class TimelineViewGui : public GuiView {
public:
    void setup(int x, int y, int w, int h, TimelineView* tv_ptr);
    
    void createComponents() override;
    void adjustComponentsSize() override;
    
    string currentTrackName;
    trackType currentTrackType;
    
    bool getFocused();
    
    void loadStateIntoSettings(TimelinePanelSettings* settings);
    void setStateFromSettings(TimelinePanelSettings& settings);
    
private:
    ofxDatGuiTextInput* gTrackNameTextInput;
    ofxDatGuiTextInput* gBpm;
    ofxDatGuiSlider* gVolumeSlider;
    ofxDatGuiToggle* gLoopToggle;
    ofxDatGuiToggle* gBpmGridToggle;
    ofxDatGuiToggle* gSnapToggle;
    ofxDatGuiToggle* gFramebasedToggle;
    
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    void onTextInputEvent(ofxDatGuiTextInputEvent e) override;
    void onDropdownEvent(ofxDatGuiDropdownEvent e) override;
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    TimelineView* timelineViewPtr;
    
    std::function<void(TimelineView*)> callback_togglePlay = &TimelineView::togglePlay;
    std::function<void(TimelineView*, float)> callback_setVolume = &TimelineView::setVolume;
    std::function<void(TimelineView*, ofLoopType)> callback_setLoopType = &TimelineView::setLoopType;
    std::function<void(TimelineView*)> callback_setIn = &TimelineView::setInPointAtPlayhead;
    std::function<void(TimelineView*)> callback_setOut = &TimelineView::setOutPointAtPlayhead;
    std::function<void(TimelineView*)> callback_addMarker = &TimelineView::addMarker;
    std::function<void(TimelineView*)> callback_clearMarkers = &TimelineView::clearMarkers;
    std::function<void(TimelineView*, bool)> callback_snap = &TimelineView::enableSnapToBPM;
    std::function<void(TimelineView*, bool)> callback_showBpmGrid = &TimelineView::setShowBPMGrid;
    std::function<void(TimelineView*, bool)> callback_framebased = &TimelineView::setFrameBased;
    
    std::function<void(TimelineView*)> callback_showTracks = &TimelineView::toggleShowTracks;
    std::function<void(TimelineView*)> callback_updateHeight = &TimelineView::updateHeight;
    std::function<void(TimelineView*, string)> callback_removeTrack = &TimelineView::removeTrack;
    std::function<void(TimelineView*, string, trackType)> callback_addTrack = &TimelineView::addTrack;
};
