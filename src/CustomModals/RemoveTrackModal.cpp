//
//  RemoveTrackModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 11/12/2018.
//

#include "RemoveTrackModal.h"

#include "GuiFactory.h"

RemoveTrackModal::RemoveTrackModal(TimelineView* timelineView_ptr){
    _timelineViewPtr = timelineView_ptr;
    setTitle("REMOVE TRACK");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &RemoveTrackModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    trackNameToRemove = "";
    
    createTogglesFromExistingTracks();
}

void RemoveTrackModal::createTogglesFromExistingTracks(){
    auto existingTracks = _timelineViewPtr->getExistingTracksRef();
    clearComponents();
    _tracksToggles.clear();
    for (auto t : existingTracks){
        auto label = t.name;
        ofxDatGuiToggle* toggle = GuiFactory::createToggle(label, false, this, &RemoveTrackModal::onToggleEvent);
        _tracksToggles.push_back(toggle);
        addComponent(toggle);
    }
}

void RemoveTrackModal::display(int height){
    createTogglesFromExistingTracks();
    setHeight(height);
    show();
}


void RemoveTrackModal::applyConfiguration(){
    if (trackNameToRemove == ""){return;}
    _timelineViewPtr->removeTrack(trackNameToRemove);
}

bool RemoveTrackModal::getFocused(){
    return false;
}
void RemoveTrackModal::onToggleEvent(ofxDatGuiButtonEvent e){
    for (auto toggle : _tracksToggles){
        toggle->setEnabled(false);
    }
    e.target->setEnabled(true);
    trackNameToRemove = e.target->getName();
}
void RemoveTrackModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
    applyConfiguration();
    hide();
}



