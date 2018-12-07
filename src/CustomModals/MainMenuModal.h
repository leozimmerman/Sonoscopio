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
#define HOST_LABEL "HOST"
#define PORT_LABEL "PORT"

class MainPanel;

class MainMenuModal : public ofxModalWindow {
public:
    
    MainMenuModal(MainPanel* mainPanel_ptr);
    
    void display(int height);
    void applySettings();
    bool getFocused();
    
    void loadStateIntoSettings(MainPanelSettings* settings);
    void setStateFromSettings(MainPanelSettings& settings);
    
private:
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    
    MainPanel* _mainPanelPtr;
    
    ofxDatGuiTextInput* gFps;
    ofxDatGuiTextInput* gHost;
    ofxDatGuiTextInput* gPort;
    
};
