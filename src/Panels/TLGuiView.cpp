//
//  GuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#include "TLGuiView.h"

ofxDatGuiDropdown* g_dropdown; //TODO: Move to header?

void TLGuiView::setup(int x, int y, int w, int h, TimelineView* tv_ptr) {

    GuiView::setup(x, y, w, h);
    timelineView_ptr = tv_ptr;

    currentTrackType = CURVES;
    currentTrackName = "";
}


void TLGuiView::createComponents(){
    
    ofxDatGuiComponent* component;
    
    gTrackName = new ofxDatGuiTextInput("TRACK NAME", "#track name#");
    component = gTrackName;
    
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &TLGuiView::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    vector<string> options = {"curves", "bangs", "switches", "notes"};
    g_dropdown = new ofxDatGuiDropdown("TRACK TYPE", options);
    //dropdown->expand();
    g_dropdown->onDropdownEvent(this, &TLGuiView::onDropdownEvent);
    component = g_dropdown;
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setStripeColor(bordCol);
    components.push_back(g_dropdown);
    
    
    component = new ofxDatGuiButton("ADD TRACK");
    component->onButtonEvent(this, &TLGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("REMOVE TRACK");
    component->onButtonEvent(this, &TLGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SHOW TRACKS");
    component->onButtonEvent(this, &TLGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("ADJUST TRACKS");
    component->onButtonEvent(this, &TLGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    
}
//--------------------------------------------------------------
void TLGuiView::adjustComponentsSize(){
    
    int guiCompWidth = _w / 7;
    int gui_y = _y;
    int gui_x = _x;
    
    ofxDatGuiComponent* component;
    //TRACK NAME
    component = components[0];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth * 2, 0.5);//width x2
    
    //TRACK TYPE
    gui_x += guiCompWidth*2;
    component = components[1];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //ADD
    gui_x += guiCompWidth;
    component = components[2];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //REMOVE
    gui_x += guiCompWidth;
    component = components[3];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //SHOW TRACKS
    gui_x += guiCompWidth;
    component = components[4];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
    //ADJUST TRACKS
    gui_x += guiCompWidth;
    component = components[5];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
}

void TLGuiView::onButtonEvent(ofxDatGuiButtonEvent e){
    //ofLogVerbose() << "onButtonEvent: " << e.target->getLabel() << "::" << e.enabled ;
    
    string label = e.target->getLabel();
    if(label ==  "ADD TRACK"){
        callback_addTrack(timelineView_ptr, currentTrackName, currentTrackType);
    }else if (label == "REMOVE TRACK"){
        callback_removeTrack(timelineView_ptr, currentTrackName);
    }else if (label == "SHOW TRACKS"){
        callback_showTracks(timelineView_ptr);
    }else if (label == "ADJUST TRACKS"){
        callback_updateHeight(timelineView_ptr);
    }
    
}

void TLGuiView::onDropdownEvent(ofxDatGuiDropdownEvent e){
    //ofLogVerbose() << "onDropdownEvent: " << e.child ;
    
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


void TLGuiView::onTextInputEvent(ofxDatGuiTextInputEvent e){
    // ofLogVerbose() << "onButtonEvent: " << e.text;
    currentTrackName = e.text;
}
