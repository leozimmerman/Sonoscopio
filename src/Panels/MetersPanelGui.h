//
//  MTGuiView.h
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#pragma once

#include "GuiView.h"
#include "MetersMenuModal.h"

#define MENU_LABEL "MENU"
#define SCROLL_UP_LABEL "<"
#define SCROLL_DOWN_LABEL ">"

class MetersPanel;

class MetersPanelGui : public GuiView {
public:
    void setup(int x, int y, int w, int h, MetersPanel* metersPanel_ptr);
    
    void createComponents() override;
    void adjustComponentsSize() override;
    
private:
    void setupMenu();
    void showMenu();
    
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    void onTextInputEvent(ofxDatGuiTextInputEvent e) override;
    void onDropdownEvent(ofxDatGuiDropdownEvent e) override;
    
    MetersPanel* _metersPanelPtr;
    shared_ptr<MetersMenuModal> menuModal;
    
};



