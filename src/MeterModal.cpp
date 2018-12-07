//
//  MeterModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 07/12/2018.
//

#include "MeterModal.h"

#include "GuiFactory.h"
#include "MeterView.h"
#include "ofApp.h"
#include "ofxAAUtils.h"
#include "PanelsBridge.h"

MeterModal::MeterModal(MeterView* meter_ptr){
    _meter_ptr = meter_ptr;
    
    auto algorithmType = _meter_ptr->getType();
    auto title = ofxaa::algorithmTypeToString(algorithmType);
    setTitle(title);
    
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MeterModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    ofxDatGuiButton *freezeBtn = GuiFactory::createButton(FREEZE_LABEL, this, &MeterModal::onButtonEvent);
    addComponent(freezeBtn);
    
    gMaxValue = GuiFactory::createTextInput(MAX_LABEL, "<max est. value>", this, &MeterModal::onTextInputEvent);
    addComponent(gMaxValue);
}

void MeterModal::display(int height){
    
    setHeight(height);
    
    show();
}

void MeterModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
    //applySettings();
    hide();
}

void MeterModal::onTextInputEvent(ofxDatGuiTextInputEvent e){}

void MeterModal::onButtonEvent(ofxDatGuiButtonEvent e){
    PanelsBridge::getInstance().freezeMeter(*_meter_ptr);
    
}

