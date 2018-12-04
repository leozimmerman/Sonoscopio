//
//  MTGuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#include "MetersPanelGuiView.h"
#include "ofApp.h"
#include "MetersPanel.h"

std::function<void(MetersPanel*)> callback_scrollUp = &MetersPanel::scrollUp;
std::function<void(MetersPanel*)> callback_scrollDown = &MetersPanel::scrollDown;


void MetersPanelGuiView::setup(int x, int y, int w, int h, MetersPanel* metersPanel_ptr){
    GuiView::setup(x, y, w, h);
    _metersPanelPtr = metersPanel_ptr;
    setupMenu();
}

void MetersPanelGuiView::setupMenu(){
    auto ofAppPtr = (ofApp*)ofGetAppPtr();
    menuModal = make_shared<MetersMenuModal>(_metersPanelPtr);
    menuModal->addListener(ofAppPtr, &ofApp::onModalEvent);
    menuModal->setMainAppPtr(ofGetAppPtr());
}

void MetersPanelGuiView::createComponents(){
    ofxDatGuiComponent* component;
    
    component = new ofxDatGuiButton("MENU");
    component->onButtonEvent(this, &MetersPanelGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(_bordCol, _bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    _components.push_back(component);
    
    component = new ofxDatGuiButton("<");
    component->onButtonEvent(this, &MetersPanelGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(_bordCol, _bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    _components.push_back(component);
    
    component = new ofxDatGuiButton(">");
    component->onButtonEvent(this, &MetersPanelGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(_bordCol, _bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    _components.push_back(component);

}

void MetersPanelGuiView::adjustComponentsSize(){
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

void MetersPanelGuiView::onButtonEvent(ofxDatGuiButtonEvent e){
    string label = e.target->getLabel();
    if(label ==  "MENU"){
        showMenu();
        
    }else if (label == "<"){
        callback_scrollUp(_metersPanelPtr);
    }else if (label == ">"){
        callback_scrollDown(_metersPanelPtr);
    }
}

void MetersPanelGuiView::onDropdownEvent(ofxDatGuiDropdownEvent e){}

void MetersPanelGuiView::onTextInputEvent(ofxDatGuiTextInputEvent e){}




void MetersPanelGuiView::showMenu(){
    menuModal->display(ofGetHeight());
}