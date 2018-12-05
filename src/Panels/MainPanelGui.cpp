//
//  MainPanelGui.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 04/12/2018.
//

#include "ofApp.h"
#include "MainPanelGui.h"
#include "MainPanel.h"
#include "GuiFactory.h"
#include "SettingsManager.h"

std::function<void(MainPanel*)> callback_saveSettings = &MainPanel::saveSettings;
std::function<void(MainPanel*)> callback_loadSettings = &MainPanel::loadSettings;

void MainPanelGui::setup(int x, int y, int w, int h, MainPanel* mainPanel_ptr){
    GuiView::setup(x, y, w, h);
    _mainPanelPtr = mainPanel_ptr;
    setupMenu();
}

void MainPanelGui::setupMenu(){
    auto ofAppPtr = (ofApp*)ofGetAppPtr();
    menuModal = make_shared<MainMenuModal>(_mainPanelPtr);
    menuModal->addListener(ofAppPtr, &ofApp::onModalEvent);
}

void MainPanelGui::showMenu(){
    menuModal->display(ofGetHeight());
}

void MainPanelGui::createComponents(){
    ofxDatGuiComponent* component;
    
    component = GuiFactory::createButton(OPEN_FILE_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(OPEN_PROJECT_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(LOAD_SETTINGS_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(SAVE_SETTINGS_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(CONFIG_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(RENDER_ANALYSIS_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createToggle(SEND_OSC_LABEL, false, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createToggle(TIME_MEASUREMENT_LABEL, false, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
 
}

void MainPanelGui::adjustComponentsSize(){
    const int guiCompHeight = GUI_COMP_HEIGHT;
    int guiCompWidth = _w / _components.size();
    int gui_y = _y;
    int gui_x = _x;
    ofxDatGuiComponent* component;
    for (int i=0; i<_components.size(); i++) {
        component = _components[i];
        component->setPosition(gui_x, gui_y);
        component->setWidth(guiCompWidth, 0.9);
        gui_x += guiCompWidth;
    }
}

void MainPanelGui::onButtonEvent(ofxDatGuiButtonEvent e){
    string label = e.target->getLabel();
    cout<<label<<endl;
    if (label == CONFIG_LABEL){
        showMenu();
    } else if (label == SAVE_SETTINGS_LABEL){
        callback_saveSettings(_mainPanelPtr);
    } else if (label == LOAD_SETTINGS_LABEL){
        callback_loadSettings(_mainPanelPtr);
    }
    
    /*
     if(e.target->getLabel()=="OPEN FILE"){
     openOpenFileDialog();
     }else if(e.target->getLabel()=="LOAD SETTINGS"){
     mMainAppPtr->loadSettings();
     }else if(e.target->getLabel()=="SAVE SETTINGS"){
     mMainAppPtr->saveSettings();
     }else if(e.target->getLabel()== "CONFIG"){
     ///showMenu();
     }else if(e.target->getLabel()== "SEND OSC"){
     mMainAppPtr->config.setIsSendingOsc(e.enabled);
     }else if(e.target->getLabel()== "SAVE ANALYSIS"){
     mMainAppPtr->saveAnalysisDataToFile();
     }
     */
}
void MainPanelGui::onTextInputEvent(ofxDatGuiTextInputEvent e){}
void MainPanelGui::onDropdownEvent(ofxDatGuiDropdownEvent e){}

void MainPanelGui::loadStateIntoSettings(MainPanelSettings* settings){
    menuModal->loadStateIntoSettings(settings);
    settings->osc.bSend = TRUE; //fix
}

void MainPanelGui::setStateFromSettings(MainPanelSettings& settings){
//    gVolumeSlider->setValue(settings.volume);
//    gLoopToggle->setEnabled(settings.bLoop);
//    gBpmGridToggle->setEnabled(settings.bBpmGrid);
//    gSnapToggle->setEnabled(settings.bSnap);
//    gFramebasedToggle->setEnabled(settings.bFrambased);
}
