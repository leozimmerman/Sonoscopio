//
//  GuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#include "ofApp.h"
#include "TimelineViewGui.h"
#include "GuiFactory.h"


void TimelineViewGui::setup(int x, int y, int w, int h, TimelineView* tv_ptr) {

    GuiView::setup(x, y, w, h);
    timelineViewPtr = tv_ptr;

    currentTrackType = CURVES;
    currentTrackName = "";
    setupMenus();
}

void TimelineViewGui::setupMenus(){
    auto ofAppPtr = (ofApp*)ofGetAppPtr();
    visibleTracksModal = make_shared<TimelineTracksModal>(timelineViewPtr);
    visibleTracksModal->addListener(ofAppPtr, &ofApp::onModalEvent);
    
    addTracksModal = make_shared<AddTrackModal>(timelineViewPtr);
    addTracksModal->addListener(ofAppPtr, &ofApp::onModalEvent);
    
    removeTracksModal = make_shared<RemoveTrackModal>(timelineViewPtr);
    removeTracksModal->addListener(ofAppPtr, &ofApp::onModalEvent);
}

void TimelineViewGui::showVisibleTracksMenu(){
    if (checkForExistingTracks()){ return; }
    visibleTracksModal->display(ofGetHeight());
}

void TimelineViewGui::showAddTrackMenu(){
    addTracksModal->display(ofGetHeight() / 2);
}

void TimelineViewGui::showRemoveTrackMenu(){
    if (checkForExistingTracks()){ return; }
    removeTracksModal->display(ofGetHeight() / 2);
}

bool TimelineViewGui::checkForExistingTracks(){
    if (timelineViewPtr->getExistingTracksRef().size() == 0){
        timelineViewPtr->showErrorMessage(NONE_EXISTING_TRACKS);
        return true;
    } else {
        return false;
    }
}

void TimelineViewGui::createComponents(){
    
    ofxDatGuiComponent* component;
    //0
    component = GuiFactory::createButton(ADD_TRACK_LABEL, this, &TimelineViewGui::onButtonEvent);
    
    _components.push_back(component);
    
    component = GuiFactory::createButton(REMOVE_TRACK_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(VISIBLE_TRACKS_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(SHOW_TRACKS_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    //--FIRST LINE:
    //6
    gBpm = GuiFactory::createTextInput(BPM_LABEL, "0", this, &TimelineViewGui::onTextInputEvent);
    _components.push_back(gBpm);
    
    gBpmGridToggle = GuiFactory::createToggle(BPM_GRID_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gBpmGridToggle);
    
    component = GuiFactory::createButton(ADD_MARKER_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(CLEAR_MARKERS_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    gSnapToggle = GuiFactory::createToggle(SNAP_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gSnapToggle);
    
    gFramebasedToggle = GuiFactory::createToggle(FRAMEBASED_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gFramebasedToggle);
    

    //--SECOND LINE:
    //12
    component = GuiFactory::createButton(REWIND_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(PLAY_STOP_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    gVolumeSlider = GuiFactory::createSlider(VOLUME_LABEL, 0.0, 1.0, 0.0, this, &TimelineViewGui::onSliderEvent);
    _components.push_back(gVolumeSlider);
    
    gLoopToggle = GuiFactory::createToggle(LOOP_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gLoopToggle);
    
    component = GuiFactory::createButton(SET_IN_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(SET_OUT_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
 
}

void TimelineViewGui::adjustComponentsSize(){
    const int guiCompHeight = GUI_COMP_HEIGHT;
    int guiCompWidth = _w / 4;
    
    ofxDatGuiComponent* component;
    
    int gui_y = _y;
    int gui_x = _x;
    for (int i=0; i<4; i++) {
        component = _components[i];
        component->setPosition(gui_x, gui_y);
        component->setWidth(guiCompWidth, 0.5);
        gui_x += guiCompWidth;
    }
    //--SECOND LINE:
    gui_y = gui_y + guiCompHeight;
    gui_x = _x;
    guiCompWidth = _w / 6;
    for (int i=4; i<10; i++) {
        component = _components[i];
        component->setPosition(gui_x, gui_y);
        component->setWidth(guiCompWidth, 0.5);
        gui_x += guiCompWidth;
    }
    
    //--THIRD LINE:
    guiCompWidth = _w / 6;
    gui_y = gui_y + guiCompHeight;
    gui_x = _x;
    for (int i=10; i<16; i++) {
        component = _components[i];
        component->setPosition(gui_x, gui_y);
        component->setWidth(guiCompWidth, 0.5);
        gui_x += guiCompWidth;
    }
    
}

void TimelineViewGui::onButtonEvent(ofxDatGuiButtonEvent e){
    string label = e.target->getLabel();
    if(label ==  ADD_TRACK_LABEL){
        showAddTrackMenu();
    }else if (label == REMOVE_TRACK_LABEL){
        showRemoveTrackMenu();
    }else if (label == SHOW_TRACKS_LABEL){
        if (!checkForExistingTracks()){
            timelineViewPtr->toggleShowTracks();
        }
    }else if (label == VISIBLE_TRACKS_LABEL){
        showVisibleTracksMenu();
    }else if(e.target->getLabel() == PLAY_STOP_LABEL){
        timelineViewPtr->togglePlay();
    }else if(e.target->getLabel() == REWIND_LABEL){
        timelineViewPtr->rewind();
    }else if(e.target->getLabel() == LOOP_LABEL){
        auto loopType = e.enabled ? OF_LOOP_NORMAL : OF_LOOP_NONE;
        timelineViewPtr->setLoopType(loopType);
    }else if(e.target->getLabel() == SET_IN_LABEL){
        timelineViewPtr->setInPointAtPlayhead();
    }else if(e.target->getLabel() ==SET_OUT_LABEL){
        timelineViewPtr->setOutPointAtPlayhead();
    }else if(e.target->getLabel()== BPM_GRID_LABEL){
        timelineViewPtr->setShowBPMGrid(e.enabled);
    }else if(e.target->getLabel()== SNAP_LABEL){
        timelineViewPtr->enableSnapToBPM(e.enabled);
    }else if(e.target->getLabel()== ADD_MARKER_LABEL){
        timelineViewPtr->addMarker();
    }else if(e.target->getLabel()== CLEAR_MARKERS_LABEL){
        timelineViewPtr->clearMarkers();
    }else if(e.target->getLabel()== FRAMEBASED_LABEL){
        timelineViewPtr->setFrameBased(e.enabled);
    }
}

void TimelineViewGui::onDropdownEvent(ofxDatGuiDropdownEvent e){
    switch (e.child) {
        case 0:
            currentTrackType = CURVES;
            break;
        case 1:
            currentTrackType = BANGS;
            break;
        case 2:
            currentTrackType = SWITCHES;
            break;
        case 3:
            currentTrackType = NOTES;
            break;
        default:
            break;
    }
}

void TimelineViewGui::onTextInputEvent(ofxDatGuiTextInputEvent e){
   if (e.target == gBpm){
        float bpm = std::stof(gBpm->getText());
        timelineViewPtr->setNewBPM(bpm);
    }
}

void TimelineViewGui::onSliderEvent(ofxDatGuiSliderEvent e){
    if (e.target->getLabel() == VOLUME_LABEL){
        timelineViewPtr->setVolume(e.value);
    }
}

void TimelineViewGui::loadStateIntoSettings(TimelinePanelSettings* settings){
    settings->volume = gVolumeSlider->getValue();
    settings->bLoop = gLoopToggle->getEnabled();
    settings->bBpmGrid = gBpmGridToggle->getEnabled();
    settings->bSnap = gSnapToggle->getEnabled();
    settings->bFrambased = gFramebasedToggle->getEnabled();
    float bpm = std::stof(gBpm->getText());
    settings->bpm = bpm;
}

void TimelineViewGui::setStateFromSettings(TimelinePanelSettings& settings){
    gVolumeSlider->setValue(settings.volume);
    gLoopToggle->setEnabled(settings.bLoop);
    gBpmGridToggle->setEnabled(settings.bBpmGrid);
    gSnapToggle->setEnabled(settings.bSnap);
    gFramebasedToggle->setEnabled(settings.bFrambased);
    gBpm->setText(ofToString(settings.bpm));
}

bool TimelineViewGui::getFocused() {
    return  gBpm->getFocused() || addTracksModal->getFocused();
}

