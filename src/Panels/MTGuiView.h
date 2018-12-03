//
//  MTGuiView.h
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#pragma once


#include "GuiView.h"
#include "MetersMenuModal.h"
#include "MetersView.h"

class MTGuiView : public GuiView {
public:
    void setup(int x, int y, int w, int h, MetersView* mt_ptr);
    
    void createComponents() override;
    void adjustComponentsSize() override;
    
private:
    void setupMenu();
    void showMenu();
    
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    void onTextInputEvent(ofxDatGuiTextInputEvent e) override;
    void onDropdownEvent(ofxDatGuiDropdownEvent e) override;
    
    MetersView* _metersViewPtr;
    shared_ptr<MetersMenuModal> menuModal;
    
    std::function<void(MetersView*)> callback_scrollUp = &MetersView::scrollUp;
    std::function<void(MetersView*)> callback_scrollDown = &MetersView::scrollDown;
    
};



