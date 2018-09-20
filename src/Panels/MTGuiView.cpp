//
//  MTGuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#include "MTGuiView.h"

void MTGuiView::setup(int x, int y, int w, int h, MetersView* mt_ptr){
    GuiView::setup(x, y, w, h);
    metersView_ptr = mt_ptr;
}

void MTGuiView::createComponents(){
    ofxDatGuiComponent* component;
    
    component = new ofxDatGuiButton("MENU");
    component->onButtonEvent(this, &MTGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("UP");
    component->onButtonEvent(this, &MTGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    component = new ofxDatGuiButton("DOWN");
    component->onButtonEvent(this, &MTGuiView::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    
}

void MTGuiView::adjustComponentsSize(){
    int gui_y = _y;
    int gui_x = _x;
    int guiCompWidth = _w / 4;
    
    ofxDatGuiComponent* component;
    //MENU
    component = components[0];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth * 2, 0.9);
    //UP
    gui_x += guiCompWidth * 2;
    component = components[1];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth , 0.9);
    //DOWN
    gui_x += guiCompWidth;
    component = components[2];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.9);
    
}

void MTGuiView::onButtonEvent(ofxDatGuiButtonEvent e){}

void MTGuiView::onDropdownEvent(ofxDatGuiDropdownEvent e){}

void MTGuiView::onTextInputEvent(ofxDatGuiTextInputEvent e){}
