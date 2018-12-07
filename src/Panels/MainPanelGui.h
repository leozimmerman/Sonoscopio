//
//  MainPanelGui.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 04/12/2018.
//

#pragma once

#include "GuiView.h"
#include "MainMenuModal.h"
#include "Settings.h"

#define OPEN_FILE_LABEL "OPEN FILE"
#define OPEN_PROJECT_LABEL "OPEN PROJECT"
#define LOAD_SETTINGS_LABEL "LOAD SETTINGS"
#define SAVE_SETTINGS_LABEL "SAVE SETTINGS"
#define CONFIG_LABEL "CONFIG"
#define RENDER_ANALYSIS_LABEL "RENDER ANALYSIS"
#define SEND_OSC_LABEL "SEND OSC"
#define TIME_MEASUREMENT_LABEL "TIME MEASUREMENT"

class MainPanel;

class MainPanelGui : public GuiView {
public:
    void setup(int x, int y, int w, int h, MainPanel* mainPanel_ptr);
    void createComponents() override;
    void adjustComponentsSize() override;
    
    bool getFocused() {
        menuModal->getFocused();
    }
    
    void loadStateIntoSettings(MainPanelSettings* settings);
    void setStateFromSettings(MainPanelSettings& settings);
    
private:
    void setupMenu();
    void showMenu();
    
    void onButtonEvent(ofxDatGuiButtonEvent e) override;
    void onTextInputEvent(ofxDatGuiTextInputEvent e) override;
    void onDropdownEvent(ofxDatGuiDropdownEvent e) override;
    
    ofxDatGuiToggle* gSendOscToggle;
    
    MainPanel* mainPanelPtr;
    shared_ptr<MainMenuModal> menuModal;
};
