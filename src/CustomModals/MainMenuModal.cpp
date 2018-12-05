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
    
    gBpm = GuiFactory::createTextInput(BPM_LABEL, "120", this, &MainMenuModal::onTextInputEvent);
    addComponent(gBpm);
    
    gHost = GuiFactory::createTextInput(HOST_LABEL, "localhost", this, &MainMenuModal::onTextInputEvent);
    addComponent(gHost);
    
    gPort = GuiFactory::createTextInput(PORT_LABEL, "12345", this, &MainMenuModal::onTextInputEvent);
    addComponent(gPort);
    
    const vector<string> buff_sizes = {"256", "512", "1024", "2048"};
    gBufferSize = GuiFactory::createDropDown(BUFFER_SIZE_LABEL, buff_sizes, this, &MainMenuModal::onBufferSizeDropdownEvent);
    addComponent(gBufferSize);
    
}


void MainMenuModal::display(int height){
    auto settings = _mainPanelPtr->getCurrentSettingsPtr();
    
    gFps->setText(std::to_string(settings->frameRate));
    selectedBufferSize = settings->bufferSize;
    gBpm->setText(std::to_string(settings->bpm));
    gHost->setText(settings->osc.host);
    gPort->setText(std::to_string(settings->osc.port));
    
    setHeight(height);
    
    show();
    
}

void MainMenuModal::applySettings(){}


void MainMenuModal::onTextInputEvent(ofxDatGuiTextInputEvent e){}

bool MainMenuModal::getFocused(){
    return (gHost->getFocused() || gPort->getFocused() || gBpm->getFocused()  || gFps->getFocused());
}

void MainMenuModal::onApplyButtonEvent(ofxDatGuiButtonEvent e) {
    applySettings();
    hide();
}

void MainMenuModal::onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e)
{
    // ofLogVerbose() << "onDropdownEvent: " << e.child << "--"<<e.target->getLabel()<<"--"<<e.parent;
    
    switch (e.child) {
            
        case 0:
            selectedBufferSize = 256;
            break;
        case 1:
            selectedBufferSize = 512;
            break;
        case 2:
            selectedBufferSize = 1024;
            break;
        case 3:
            selectedBufferSize = 2048;
            break;
            
        default:
            break;
    }
    
}

void MainMenuModal::loadStateIntoSettings(MainPanelSettings* settings){
    try {
        int fps = std::stoi(gFps->getText());
        int bufferSize = selectedBufferSize;
        float bpm = std::stof(gBpm->getText());
        string host = gHost->getText();
        int port = std::stoi(gPort->getText());
    
        settings->frameRate = fps;
        settings->bufferSize = bufferSize;
        settings->bpm = bpm;
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
