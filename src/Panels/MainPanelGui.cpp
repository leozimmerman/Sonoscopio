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

void MainPanelGui::setup(int x, int y, int w, int h, MainPanel* mainPanel_ptr){
    GuiView::setup(x, y, w, h);
    mainPanelPtr = mainPanel_ptr;
    setupMenu();
}

void MainPanelGui::setupMenu(){
    auto ofAppPtr = (ofApp*)ofGetAppPtr();
    menuModal = make_shared<MainMenuModal>(mainPanelPtr);
    menuModal->addListener(ofAppPtr, &ofApp::onModalEvent);
}

void MainPanelGui::showMenu(){
    menuModal->display(ofGetHeight());
}

void MainPanelGui::createComponents(){
    ofxDatGuiComponent* component;
    
    component = GuiFactory::createButton(OPEN_FILE_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(SAVE_SETTINGS_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(CONFIG_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    component = GuiFactory::createButton(RENDER_ANALYSIS_LABEL, this, &MainPanelGui::onButtonEvent);
    _components.push_back(component);
    gSendOscToggle = GuiFactory::createToggle(SEND_OSC_LABEL, false, this, &MainPanelGui::onButtonEvent);
    _components.push_back(gSendOscToggle);
 
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
    if (label == CONFIG_LABEL){
        showMenu();
    }else if (label == SAVE_SETTINGS_LABEL){
        mainPanelPtr->saveAllSettings();
    }else if (label == OPEN_FILE_LABEL){
        mainPanelPtr->openFileDialog();
    }else if (label == RENDER_ANALYSIS_LABEL){
        mainPanelPtr->renderAnalysis();
    }
}
void MainPanelGui::onTextInputEvent(ofxDatGuiTextInputEvent e){}
void MainPanelGui::onDropdownEvent(ofxDatGuiDropdownEvent e){}

void MainPanelGui::loadStateIntoSettings(MainPanelSettings* settings){
    menuModal->loadStateIntoSettings(settings);
    settings->osc.bSend = gSendOscToggle->getEnabled();
}

void MainPanelGui::setStateFromSettings(MainPanelSettings& settings){
    menuModal->setStateFromSettings(settings);
    gSendOscToggle->setEnabled(settings.osc.bSend);
}
