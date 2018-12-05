//
//  CustomModals.hpp
//  Sonoscopio
//
//  Created by Leo on 12/15/17.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"
#include "Settings.h"

#define SET_FPS_LABEL "SET FPS"
#define BPM_LABEL "BPM"
#define HOST_LABEL "HOST"
#define PORT_LABEL "PORT"
#define BUFFER_SIZE_LABEL "BUFFER SIZE"

class MainPanel;

class MainMenuModal : public ofxModalWindow {
public:
    
    MainMenuModal(MainPanel* mainPanel_ptr);
    
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e);
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    
    void display(int height);
    void applySettings();
    bool getFocused();
    
    void loadStateIntoSettings(MainPanelSettings* settings);
    void setStateFromSettings(MainPanelSettings& settings);
    
private:
    MainPanel* _mainPanelPtr;
    
    ofxDatGuiTextInput* gFps;
    ofxDatGuiDropdown* gBufferSize;
    ofxDatGuiTextInput* gBpm;
    ofxDatGuiTextInput* gHost;
    ofxDatGuiTextInput* gPort;
    
    int selectedBufferSize;
    
};
