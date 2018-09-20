//
//  GuiView.hpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#pragma once

#include "GuiView.h"
#include "TimelineView.h"


class TLGuiView : public GuiView {
public:
    void setup(int x, int y, int w, int h, TimelineView* tv_ptr);
    
    void createComponents() override;
    void adjustComponentsSize() override;
    
    string currentTrackName;
    trackType currentTrackType;
    ofxDatGuiTextInput* gTrackName;
    
    
private:
    
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    void onTextInputEvent(ofxDatGuiTextInputEvent e) override;
    void onDropdownEvent(ofxDatGuiDropdownEvent e) override;
    
    TimelineView* timelineView_ptr;
    
    std::function<void(TimelineView*)> callback_showTracks = &TimelineView::toggleShowTracks;
    std::function<void(TimelineView*)> callback_updateHeight = &TimelineView::updateHeight;
    std::function<void(TimelineView*, string)> callback_removeTrack = &TimelineView::removeTrack;
    std::function<void(TimelineView*, string, trackType)> callback_addTrack = &TimelineView::addTrack;
};
