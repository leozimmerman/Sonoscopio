//
//  RemoveTrackModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 11/12/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"
#include "Settings.h"
#include "TimelineView.h"

class RemoveTrackModal : public ofxModalWindow {
public:
    
    RemoveTrackModal(TimelineView* timelineView_ptr);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    

private:
    void createTogglesFromExistingTracks();
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiButtonEvent e);
    
    vector<ofxDatGuiToggle*> _tracksToggles;
    TimelineView* _timelineViewPtr;
    
    string trackNameToRemove;
};
