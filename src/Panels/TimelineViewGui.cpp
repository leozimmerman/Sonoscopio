//
//  GuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#include "TimelineViewGui.h"
#include "GuiFactory.h"


void TimelineViewGui::setup(int x, int y, int w, int h, TimelineView* tv_ptr) {

    GuiView::setup(x, y, w, h);
    timelineViewPtr = tv_ptr;

    currentTrackType = CURVES;
    currentTrackName = "";
}

void TimelineViewGui::createComponents(){
    
    ofxDatGuiComponent* component;
    //0
    component = GuiFactory::createButton(PLAY_STOP_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    gVolumeSlider = GuiFactory::createSlider(VOLUME_LABEL, 0.0, 1.0, 1.0, this, &TimelineViewGui::onSliderEvent);
    _components.push_back(gVolumeSlider);
    
    gLoopToggle = GuiFactory::createToggle(LOOP_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gLoopToggle);
    
    component = GuiFactory::createButton(SET_IN_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(SET_OUT_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(ADD_MARKER_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(CLEAR_MARKERS_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    gBpmGridToggle = GuiFactory::createToggle(BPM_GRID_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gBpmGridToggle);
    
    gSnapToggle = GuiFactory::createToggle(SNAP_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gSnapToggle);
    //9
    gFramebasedToggle = GuiFactory::createToggle(FRAMEBASED_LABEL, false, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(gFramebasedToggle);
    //Second line
    //10
    gTrackNameTextInput = GuiFactory::createTextInput(TRACK_NAME_LABEL, "<track name>", this, &TimelineViewGui::onTextInputEvent);
    _components.push_back(gTrackNameTextInput);
    
    vector<string> options = {"curves", "bangs", "switches", "notes"};
    component = GuiFactory::createDropDown(TRACK_TYPE_LABEL, options, this, &TimelineViewGui::onDropdownEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(ADD_TRACK_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(REMOVE_TRACK_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(SHOW_TRACKS_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(ADJUST_TRACKS_LABEL, this, &TimelineViewGui::onButtonEvent);
    _components.push_back(component);
    
}

void TimelineViewGui::adjustComponentsSize(){
    const int guiCompHeight = GUI_COMP_HEIGHT;
    int guiCompWidth = _w / 5;
    
    ofxDatGuiComponent* component;
    
    int gui_y = _y;
    int gui_x = _x;
    for (int i=0; i<5; i++) {
        component = _components[i];
        component->setPosition(gui_x, gui_y);
        component->setWidth(guiCompWidth, 0.5);
        gui_x += guiCompWidth;
    }
    //--SECOND LINE:
    gui_y = _y + guiCompHeight;
    gui_x = _x;
    for (int i=5; i<10; i++) {
        component = _components[i];
        component->setPosition(gui_x, gui_y);
        component->setWidth(guiCompWidth, 0.5);
        gui_x += guiCompWidth;
    }
    
    //--THIRD LINE:
    guiCompWidth = _w / 7;
    gui_y = _y + guiCompHeight *2;
    gui_x = _x;
    //TRACK NAME
    component = _components[10];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth * 2, 0.5);//width x2
    
    //TRACK TYPE
    gui_x += guiCompWidth*2;
    component = _components[11];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //ADD
    gui_x += guiCompWidth;
    component = _components[12];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //REMOVE
    gui_x += guiCompWidth;
    component = _components[13];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //SHOW TRACKS
    gui_x += guiCompWidth;
    component = _components[14];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //ADJUST TRACKS
    gui_x += guiCompWidth;
    component = _components[15];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
}

void TimelineViewGui::onButtonEvent(ofxDatGuiButtonEvent e){
    string label = e.target->getLabel();
    if(label ==  ADD_TRACK_LABEL){
        callback_addTrack(timelineViewPtr, currentTrackName, currentTrackType);
    }else if (label == REMOVE_TRACK_LABEL){
        callback_removeTrack(timelineViewPtr, currentTrackName);
    }else if (label == SHOW_TRACKS_LABEL){
        callback_showTracks(timelineViewPtr);
    }else if (label == ADJUST_TRACKS_LABEL){
        callback_updateHeight(timelineViewPtr);
    }else if(e.target->getLabel() == PLAY_STOP_LABEL){
        callback_togglePlay(timelineViewPtr);
    }else if(e.target->getLabel() == LOOP_LABEL){
        auto loopType = e.enabled ? OF_LOOP_NORMAL : OF_LOOP_NONE;
        callback_setLoopType(timelineViewPtr, loopType);
    }else if(e.target->getLabel() == SET_IN_LABEL){
        callback_setIn(timelineViewPtr);
    }else if(e.target->getLabel() ==SET_OUT_LABEL){
        callback_setOut(timelineViewPtr);
    }else if(e.target->getLabel()== BPM_GRID_LABEL){
        callback_showBpmGrid(timelineViewPtr, e.enabled);
    }else if(e.target->getLabel()== SNAP_LABEL){
        callback_snap(timelineViewPtr, e.enabled);
    }else if(e.target->getLabel()== ADD_MARKER_LABEL){
        callback_addMarker(timelineViewPtr);
    }else if(e.target->getLabel()== CLEAR_MARKERS_LABEL){
        callback_clearMarkers(timelineViewPtr);
    }else if(e.target->getLabel()== FRAMEBASED_LABEL){
        callback_framebased(timelineViewPtr, e.enabled);
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
    // ofLogVerbose() << "onButtonEvent: " << e.text;
    currentTrackName = e.text;
}

void TimelineViewGui::onSliderEvent(ofxDatGuiSliderEvent e){
    if (e.target->getLabel() == VOLUME_LABEL){
        callback_setVolume(timelineViewPtr, e.value);
    }
}

void TimelineViewGui::loadStateIntoSettings(TimelinePanelSettings* settings){
    settings->volume = gVolumeSlider->getValue();
    settings->bLoop = gLoopToggle->getEnabled();
    settings->bBpmGrid = gBpmGridToggle->getEnabled();
    settings->bSnap = gSnapToggle->getEnabled();
    settings->bFrambased = gFramebasedToggle->getEnabled();
}

void TimelineViewGui::setStateFromSettings(TimelinePanelSettings& settings){
    gVolumeSlider->setValue(settings.volume);
    gLoopToggle->setEnabled(settings.bLoop);
    gBpmGridToggle->setEnabled(settings.bBpmGrid);
    gSnapToggle->setEnabled(settings.bSnap);
    gFramebasedToggle->setEnabled(settings.bFrambased);
}



