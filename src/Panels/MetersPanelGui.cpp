//
//  MTGuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#include "ofApp.h"
#include "MetersPanelGui.h"
#include "MetersPanel.h"
#include "GuiFactory.h"


std::function<void(MetersPanel*)> callback_scrollUp = &MetersPanel::scrollUp;
std::function<void(MetersPanel*)> callback_scrollDown = &MetersPanel::scrollDown;


void MetersPanelGui::setup(int x, int y, int w, int h, MetersPanel* metersPanel_ptr){
    GuiView::setup(x, y, w, h);
    _metersPanelPtr = metersPanel_ptr;
    setupMenu();
}

void MetersPanelGui::setupMenu(){
    auto ofAppPtr = (ofApp*)ofGetAppPtr();
    menuModal = make_shared<MetersMenuModal>(_metersPanelPtr);
    menuModal->addListener(ofAppPtr, &ofApp::onModalEvent);
}

void MetersPanelGui::showMenu(){
    menuModal->display(ofGetHeight());
}

void MetersPanelGui::createComponents(){
    ofxDatGuiComponent* component;
    
    component = GuiFactory::createButton(MENU_LABEL, this, &MetersPanelGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(SCROLL_UP_LABEL, this, &MetersPanelGui::onButtonEvent);
    _components.push_back(component);
    
    component = GuiFactory::createButton(SCROLL_DOWN_LABEL, this, &MetersPanelGui::onButtonEvent);
    _components.push_back(component);

}

void MetersPanelGui::adjustComponentsSize(){
    int gui_y = _y;
    int gui_x = _x;
    int guiCompWidth = _w / 4;
    
    ofxDatGuiComponent* component;
    //MENU
    component = _components[0];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth * 2, 0.9);
    //UP
    gui_x += guiCompWidth * 2;
    component = _components[1];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth , 0.9);
    //DOWN
    gui_x += guiCompWidth;
    component = _components[2];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
}

void MetersPanelGui::onButtonEvent(ofxDatGuiButtonEvent e){
    string label = e.target->getLabel();
    if(label ==  MENU_LABEL){
        showMenu();
        
    }else if (label == SCROLL_UP_LABEL){
        callback_scrollUp(_metersPanelPtr);
    }else if (label == SCROLL_DOWN_LABEL){
        callback_scrollDown(_metersPanelPtr);
    }
}

void MetersPanelGui::onDropdownEvent(ofxDatGuiDropdownEvent e){}

void MetersPanelGui::onTextInputEvent(ofxDatGuiTextInputEvent e){}

