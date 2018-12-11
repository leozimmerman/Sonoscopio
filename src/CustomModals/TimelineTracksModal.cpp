//
//  TimelineTracksModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 10/12/2018.
//

#include "TimelineTracksModal.h"
#include "GuiFactory.h"

TimelineTracksModal::TimelineTracksModal(TimelineView* timelineView_ptr){
    _timelineViewPtr = timelineView_ptr;
    setTitle("VISIBLE TRACKS");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &TimelineTracksModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
   
    createTogglesFromExistingTracks();
}

void TimelineTracksModal::createTogglesFromExistingTracks(){
    auto availableTracks = _timelineViewPtr->getExistingTracksRef();
    clearComponents();
    _tracksToggles.clear();
    for (int i=0; i<availableTracks.size(); i++){
        auto label = availableTracks[i].name;
        ofxDatGuiToggle* toggle = GuiFactory::createToggle(label, TRUE, this, &TimelineTracksModal::onToggleEvent);
        _tracksToggles.push_back(toggle);
        addComponent(toggle);
    }
}

void TimelineTracksModal::display(int height){
    createTogglesFromExistingTracks();
    updateVisibleTracks();
    updateTogglesFromVisibleTracks();
    
    setHeight(height);
    show();
}

void TimelineTracksModal::updateTogglesFromVisibleTracks(){
    auto availableTracks = _timelineViewPtr->getExistingTracksRef();
    if (_tracksToggles.size() != availableTracks.size()){
        return;
    }
    
    for (int i=0; i<_tracksToggles.size(); i++){
        bool enabled = isTrackVisible(availableTracks[i]);
        _tracksToggles[i]->setEnabled(enabled);
    }
}

void TimelineTracksModal::updateVisibleTracksFromToggles(){
    auto availableTracks = _timelineViewPtr->getExistingTracksRef();
    if (_tracksToggles.size() != availableTracks.size()){
        return;
    }
    visibleTracks.clear();
    for (int i=0; i<_tracksToggles.size(); i++){
        if (_tracksToggles[i]->getEnabled()) {
            visibleTracks.push_back(availableTracks[i]);
        }
    }
}

bool TimelineTracksModal::isTrackVisible(TLTrackSetting track){
    for(auto vTrack : visibleTracks){
        if (vTrack.name == track.name) {
            return true;
        }
    }
    return false;
}

void TimelineTracksModal::applyConfiguration(){
    updateVisibleTracksFromToggles();
    _timelineViewPtr->setVisibleTracks(visibleTracks);
}

bool TimelineTracksModal::getFocused(){
    return false;
}
void TimelineTracksModal::onToggleEvent(ofxDatGuiButtonEvent e){
    //TODO: Implement?
}
void TimelineTracksModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
    applyConfiguration();
    hide();
}

void TimelineTracksModal::loadStateIntoSettings(TimelinePanelSettings* settings){
    //settings->bufferSize = selectedBufferSize;
    
}

void TimelineTracksModal::updateVisibleTracks(){
    visibleTracks = _timelineViewPtr->getVisibleTracksRef();
    updateTogglesFromVisibleTracks();
}

