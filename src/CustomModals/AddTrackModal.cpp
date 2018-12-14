//
//  AddTrackModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 11/12/2018.
//

#include "AddTrackModal.h"

#include "GuiFactory.h"

AddTrackModal::AddTrackModal(TimelineView* timelineView_ptr){
    _timelineViewPtr = timelineView_ptr;
    setTitle("ADD NEW TRACK");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &AddTrackModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    gTrackNameTextInput = GuiFactory::createTextInput(TRACK_NAME_LABEL, "<track name>", this, &AddTrackModal::onTextInputEvent);
    addComponent(gTrackNameTextInput);
    
    ofxDatGuiDropdown* dropdown = GuiFactory::createDropDown(TRACK_TYPE_LABEL, options, this, &AddTrackModal::onDropdownEvent);
    addComponent(dropdown);
    
    selectedTrackName = "";
    selectedTrackType = options[0];
}

void AddTrackModal::display(int height){
    setHeight(height);
    show();
}

void AddTrackModal::applyConfiguration(){
    if (selectedTrackName == "" || selectedTrackType == ""){return;}
    _timelineViewPtr->createNewTrack(selectedTrackName, selectedTrackType);
}

bool AddTrackModal::getFocused(){
    return gTrackNameTextInput->getFocused();
}

void AddTrackModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
    applyConfiguration();
    hide();
}

void AddTrackModal::onTextInputEvent(ofxDatGuiTextInputEvent e){
    selectedTrackName = e.text;
}

void AddTrackModal::onDropdownEvent(ofxDatGuiDropdownEvent e){
    selectedTrackType = options[e.child];
}


