//
//  MetersMenuModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 30/11/2018.
//

#include "MetersMenuModal.h"
#include "ofxAAUtils.h"
#include "MetersPanel.h"

std::function<void(MetersPanel*, vector<ofxAAAlgorithmType>&)> callback_setEnabledAlgorithms = &MetersPanel::setEnabledAlgorithms;

MetersMenuModal::MetersMenuModal(MetersPanel* metersPanel_ptr){
    _metersPanelPtr = metersPanel_ptr;
    setTitle("METERS MENU");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MetersMenuModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    auto availableAlgorithms = ofxaa::allAvailableAlgorithmTypes;
    
    for (int i=0; i<availableAlgorithms.size(); i++){
        auto label = ofxaa::algorithmTypeToString(availableAlgorithms[i]);
        ofxDatGuiToggle* toggle = new ofxDatGuiToggle(label, TRUE);
        toggle->setLabelAlignment(ofxDatGuiAlignment::LEFT);
        toggle->onButtonEvent(this, &MetersMenuModal::onToggleEvent);
        toggle->setBorderVisible(TRUE);
        toggle->setStripeVisible(false);
        _algorithmToggles.push_back(toggle);
        addComponent(toggle);
    }
}

void MetersMenuModal::display(int height){
    setHeight(height);
    show();
}

void MetersMenuModal::applyConfiguration(){
    auto availableAlgorithms = ofxaa::allAvailableAlgorithmTypes;
    if (_algorithmToggles.size() != availableAlgorithms.size()){
        return;
    }
    vector<ofxAAAlgorithmType> enabledAlgorithms;
    for (int i=0; i<_algorithmToggles.size(); i++){
        if (_algorithmToggles[i]->getEnabled()) {
            enabledAlgorithms.push_back(availableAlgorithms[i]);
        }
    }
    
    callback_setEnabledAlgorithms(_metersPanelPtr, enabledAlgorithms);
}

bool MetersMenuModal::getFocused(){
    return false;
}
void MetersMenuModal::onToggleEvent(ofxDatGuiButtonEvent e){
    //TODO: Implement?
}
void MetersMenuModal::onApplyButtonEvent(ofxDatGuiButtonEvent e) {
    applyConfiguration();
    hide();
}
