//
//  MainPanelGui.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 04/12/2018.
//

#pragma once

#include "GuiView.h"
#include "MainMenuModal.h"

#define OPEN_FILE_LABEL "OPEN FILE"
#define OPEN_PROJECT_LABEL "OPEN PROJECT"
#define LOAD_SETTINGS_LABEL "LOAD SETTINGS"
#define SAVE_SETTINGS_LABEL "SAVE SETTINGS"
#define CONFIG_LABEL "CONFIG"
#define RENDER_ANALYSIS_LABEL "RENDER ANALYSIS"
#define SEND_OSC_LABEL "SEND OSC"

class MainPanel;

class MainPanelGui : public GuiView {
public:
    void setup(int x, int y, int w, int h, MainPanel* mainPanel_ptr);
    void createComponents() override;
    void adjustComponentsSize() override;
    
    bool getFocused() {
        menuModal->getFocused();
    }
    
private:
    void setupMenu();
    void showMenu();
    
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    void onTextInputEvent(ofxDatGuiTextInputEvent e) override;
    void onDropdownEvent(ofxDatGuiDropdownEvent e) override;
    
    MainPanel* _mainPanelPtr;
    shared_ptr<MainMenuModal> menuModal;
};
