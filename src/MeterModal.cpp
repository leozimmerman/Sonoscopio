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
    meterView_ptr = meter_ptr;
    
    auto title = meter_ptr->getName();
    setTitle(title);
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MeterModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    gPlotValueToggle = GuiFactory::createToggle(PLOT_VALUE_LABEL, false, this, &MeterModal::onButtonEvent);

    addComponent(gPlotValueToggle);
    
    gMaxValueTextInput = GuiFactory::createTextInput(MAX_LABEL, "<max est. value>", this, &MeterModal::onTextInputEvent);
    addComponent(gMaxValueTextInput);
    
}

void MeterModal::display(int height){
    float maxExtValue = meterView_ptr->getMaxEstimatedValue();
    if (maxExtValue == 0.0){
        gMaxValueTextInput->setVisible(false);
    } else {
        gMaxValueTextInput->setVisible(true);
        gMaxValueTextInput->setText(std::to_string(int(maxExtValue)));
    }
    gPlotValueToggle->setEnabled(meterView_ptr->getPlotterEnabled());
    setHeight(height);
    show();
}

void MeterModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
    applySettings();
    hide();
}

void MeterModal::applySettings(){
    meterView_ptr->toggleValuePlotter(gPlotValueToggle->getEnabled());
    if (gMaxValueTextInput->getVisible()){
        try {
            float maxValue = std::stof(gMaxValueTextInput->getText());
            meterView_ptr->setMaxEstimatedValue(maxValue);
        } catch (const std::invalid_argument& ia) {
            std::cerr << "Metar View Modal Invalid arguments: " << ia.what() << '\n';
            string message = "Invalid value";
            ofNotifyEvent(ofApp::errorEvent, message);
        }
    }
}

void MeterModal::onTextInputEvent(ofxDatGuiTextInputEvent e){}
void MeterModal::onButtonEvent(ofxDatGuiButtonEvent e){}

void MeterModal::loadStateIntoSettings(MainPanelSettings* settings){}
void MeterModal::setStateFromSettings(MainPanelSettings& settings){}


