//
//  MTGuiView.h
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#pragma once

#include "GuiView.h"
#include "MetersMenuModal.h"
#include "Settings.h"
#define MENU_LABEL "ANALYSIS MENU"
#define SCROLL_UP_LABEL "<"
#define SCROLL_DOWN_LABEL ">"

class MetersPanel;

class MetersPanelGui : public GuiView {
public:
    void setup(int x, int y, int w, int h, MetersPanel* metersPanel_ptr);
    ~MetersPanelGui();
    
    void createComponents() override;
    void adjustComponentsSize() override;
    
    void loadStateIntoSettings(MetersPanelSettings* settings);
    void setStateFromSettings(MetersPanelSettings& settings);
    
    bool getFocused() override;
    
private:
    void setupMenu();
    void showMenu();
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    MetersPanel* _metersPanelPtr;
    shared_ptr<MetersMenuModal> menuModal;
    
};



