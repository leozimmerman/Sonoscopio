//
//  GuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#include "GuiView.h"

ofxDatGuiDropdown* g_dropdown; //TODO: Move to header?

void GuiView::setup(int x, int y, int width, int height, TimelineView* tv_ptr) {
    View::setup(x, y, width, height);
    setBackgroundColor(ofColor::orange);
    bordCol = ofColor::grey;
    bordWidth = 1;
    _guiCompHeight = height;
    
    currentTrackType = CURVES;
    currentTrackName = "";
    
    setupGui();
    
    timelineView_ptr = tv_ptr;
}

void GuiView::update(){
    for(int i=0; i<components.size(); i++){
        components[i]->update();
    }
}
void GuiView::draw(){
    View::draw();
    for(int i=0; i<components.size(); i++){
        components[i]->draw();
    }
}
void GuiView::resize(int x, int y, int width, int height){
    View::resize(x, y, width, height);
    adjustGuiSize();
}
void GuiView::setupGui(){
    
    ofxDatGuiComponent* component;
    
    gTrackName = new ofxDatGuiTextInput("TRACK NAME", "#track name#");
    component = gTrackName;
    
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &GuiView::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    vector<string> options = {"curves", "bangs", "switches", "notes"};
    g_dropdown = new ofxDatGuiDropdown("TRACK TYPE", options);
    //dropdown->expand();
    g_dropdown->onDropdownEvent(this, &GuiView::onDropdownEvent);
    component = g_dropdown;
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setStripeColor(bordCol);
    components.push_back(g_dropdown);
    
    
    component = new ofxDatGuiButton("ADD TRACK");
    component->onButtonEvent(this, &GuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("REMOVE TRACK");
    component->onButtonEvent(this, &GuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("SHOW TRACKS");
    component->onButtonEvent(this, &GuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("ADJUST TRACKS");
    component->onButtonEvent(this, &GuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    //-:Set components Positions and Widths
    adjustGuiSize();
    
}
//--------------------------------------------------------------
void GuiView::adjustGuiSize(){
    
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

void GuiView::onButtonEvent(ofxDatGuiButtonEvent e){
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

void GuiView::onDropdownEvent(ofxDatGuiDropdownEvent e){
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


void GuiView::onTextInputEvent(ofxDatGuiTextInputEvent e){
    // ofLogVerbose() << "onButtonEvent: " << e.text;
    currentTrackName = e.text;
}
