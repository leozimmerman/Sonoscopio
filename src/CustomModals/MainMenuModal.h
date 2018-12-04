//
//  CustomModals.hpp
//  Sonoscopio
//
//  Created by Leo on 12/15/17.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"

class MainPanel;

class MainMenuModal : public ofxModalWindow {
public:
    
    MainMenuModal(MainPanel* mainPanel_ptr);
    
    
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e);
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
    string getHostText(){ return gHost->getText(); }
    string getPortText(){ return gPort->getText(); }
    string getBpmText() { return gBpm->getText(); }
    string getFpsText(){ return gFps->getText(); }
    string getBufferSizeText(){ return gBufferSize->getSelected()->getName(); }
    
private:
    MainPanel* _mainPanelPtr;
    
    ofxDatGuiTextInput* gHost;
    ofxDatGuiTextInput* gPort;
    ofxDatGuiTextInput* gBpm;
    ofxDatGuiTextInput* gFps;
    ofxDatGuiDropdown* gBufferSize;
    
    int selectedBufferSize;
    
};
