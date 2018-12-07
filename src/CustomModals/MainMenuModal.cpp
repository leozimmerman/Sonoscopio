//
//  CustomModals.cpp
//  Sonoscopio
//
//  Created by Leo on 12/15/17.
//

#include "MainMenuModal.h"
#include "GuiFactory.h"
#include "MainPanel.h"

//std::function<void(MainPanel*, int, int, int, string, int)> callback_applySettings = &MainPanel::applySettings;

MainMenuModal::MainMenuModal(MainPanel* mainPanel_ptr){
    _mainPanelPtr = mainPanel_ptr;
    
    setTitle("CONFIGURATION");
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MainMenuModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    //ofxDatGuiComponent* component;
    gFps = GuiFactory::createTextInput(SET_FPS_LABEL, ofToString(INIT_FPS), this, &MainMenuModal::onTextInputEvent);
    addComponent(gFps);
    
    gHost = GuiFactory::createTextInput(HOST_LABEL, "localhost", this, &MainMenuModal::onTextInputEvent);
    addComponent(gHost);
    
    gPort = GuiFactory::createTextInput(PORT_LABEL, "12345", this, &MainMenuModal::onTextInputEvent);
    addComponent(gPort);
    
}


void MainMenuModal::display(int height){
    auto settings = _mainPanelPtr->getCurrentSettingsPtr();
    gFps->setText(std::to_string(settings->frameRate));
    gHost->setText(settings->osc.host);
    gPort->setText(std::to_string(settings->osc.port));
    
    setHeight(height);
    
    show();
}

void MainMenuModal::applySettings(){}


void MainMenuModal::onTextInputEvent(ofxDatGuiTextInputEvent e){}

bool MainMenuModal::getFocused(){
    return (gHost->getFocused() || gPort->getFocused() || gFps->getFocused());
}

void MainMenuModal::onApplyButtonEvent(ofxDatGuiButtonEvent e) {
    applySettings();
    hide();
}

void MainMenuModal::loadStateIntoSettings(MainPanelSettings* settings){
    try {
        int fps = std::stoi(gFps->getText());
        string host = gHost->getText();
        int port = std::stoi(gPort->getText());
    
        settings->frameRate = fps;
        settings->osc.host = host;
        settings->osc.port = port;
        
    } catch (const std::invalid_argument& ia) {
        //TODO: Show error modal
        std::cerr << "Main Menu Modal Invalid arguments: " << ia.what() << '\n';
    }
  

}

void MainMenuModal::setStateFromSettings(MainPanelSettings& settings){
    //    gVolumeSlider->setValue(settings.volume);
    //    gLoopToggle->setEnabled(settings.bLoop);
    //    gBpmGridToggle->setEnabled(settings.bBpmGrid);
    //    gSnapToggle->setEnabled(settings.bSnap);
    //    gFramebasedToggle->setEnabled(settings.bFrambased);
}
