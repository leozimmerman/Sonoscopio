//
//  MainPanelGui.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 04/12/2018.
//

#pragma once

#include "GuiView.h"
#include "MainMenuModal.h"
#include "AnalysisSaverModal.h"
#include "Settings.h"

#define OPEN_FILE_LABEL "OPEN FILE"
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
    
    void loadStateIntoSettings(MainPanelSettings* settings);
    void setStateFromSettings(MainPanelSettings& settings);
    
    bool getFocused() override{ return configModal->getFocused(); }
    bool getOscEnabled(){ return  gSendOscToggle->getEnabled(); }
    
private:
    void setupConfigMenu();
    void showConfigMenu();
    void setupSaverMenu();
    void showSaverMenu();
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    ofxDatGuiToggle* gSendOscToggle;
    
    MainPanel* mainPanelPtr;
    shared_ptr<MainMenuModal> configModal;
    shared_ptr<AnalysisSaverModal> saverModal;
};
