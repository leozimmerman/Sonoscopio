//
//  MTGuiView.hpp
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#pragma once

#include "GuiView.h"
#include "MetersView.h"

class MTGuiView : public GuiView {
public:
    void setup(int x, int y, int w, int h, MetersView* mt_ptr);
    
    void createComponents() override;
    void adjustComponentsSize() override;
    
private:
    
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    void onTextInputEvent(ofxDatGuiTextInputEvent e) override;
    void onDropdownEvent(ofxDatGuiDropdownEvent e) override;
    
    MetersView* metersView_ptr;
    
    /*(
    std::function<void(TimelineView*)> callback_showTracks = &TimelineView::toggleShowTracks;
    std::function<void(TimelineView*)> callback_updateHeight = &TimelineView::updateHeight;
    std::function<void(TimelineView*, string)> callback_removeTrack = &TimelineView::removeTrack;
    std::function<void(TimelineView*, string, trackType)> callback_addTrack = &TimelineView::addTrack;
    */
};
