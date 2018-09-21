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
    
    
    std::function<void(MetersView*)> callback_scrollUp = &MetersView::scrollUp;
    std::function<void(MetersView*)> callback_scrollDown = &MetersView::scrollDown;
    
};
