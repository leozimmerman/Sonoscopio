//
//  GuiView.hpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "TimelineView.h"


class GuiView : public View {
public:
    void setup(int x, int y, int width, int height, TimelineView* tv_ptr);
    void update();
    void draw() override;
    
    void resize(int x, int y, int width, int height) override;
    void setupGui();
    void adjustGuiSize();
    
    
    int componentHeight;
    
    vector<ofxDatGuiComponent*> components;
    bool _isHidden = false;
    ofColor bordCol;
    int bordWidth;
    int _guiCompHeight;
    int  _guiCompWidth;
    
    ofxDatGuiTextInput* gTrackName;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    string currentTrackName;
    trackType currentTrackType;
    
    
private:
    
    TimelineView* timelineView_ptr;
    std::function<void(TimelineView*)> callback_showTracks = &TimelineView::toggleShowTracks;
    std::function<void(TimelineView*)> callback_updateHeight = &TimelineView::updateHeight;
    std::function<void(TimelineView*, string)> callback_removeTrack = &TimelineView::removeTrack;
    std::function<void(TimelineView*, string, trackType)> callback_addTrack = &TimelineView::addTrack;
};
