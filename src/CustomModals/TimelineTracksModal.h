//
//  TimelineTracksModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 10/12/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"
#include "Settings.h"
#include "TimelineView.h"

class TimelineTracksModal : public ofxModalWindow {
public:
    
    TimelineTracksModal(TimelineView* timelineView_ptr);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
    void loadStateIntoSettings(TimelinePanelSettings* settings);
    
        
    void updateTogglesFromVisibleTracks();
    void updateVisibleTracksFromToggles();
    
private:
    void createTogglesFromExistingTracks();
    void updateVisibleTracks();
    bool isTrackVisible(TLTrackSetting track);
    
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiButtonEvent e);
    
    vector<ofxDatGuiToggle*> _tracksToggles;
    
    vector<TLTrackSetting> visibleTracks;
    //vector<TLTrackSetting> allExistingTracks;
    TimelineView* _timelineViewPtr;
};
