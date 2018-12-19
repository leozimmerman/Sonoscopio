//
//  MetersMenuModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 30/11/2018.
//

#include "MetersMenuModal.h"
#include "ofxAAUtils.h"
#include "MetersPanel.h"
#include "GuiFactory.h"

MetersMenuModal::MetersMenuModal(MetersPanel* metersPanel_ptr){
    _metersPanelPtr = metersPanel_ptr;
    setTitle("METERS MENU");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MetersMenuModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    selectedBufferSize = -1;
    
    auto availableAlgorithms = ofxaa::allAvailableAlgorithmTypes;
    
    for (int i=0; i<availableAlgorithms.size(); i++){
        auto label = ofxaa::algorithmTypeToString(availableAlgorithms[i]);
        ofxDatGuiToggle* toggle = GuiFactory::createToggle(label, TRUE, this, &MetersMenuModal::onToggleEvent);
        _algorithmToggles.push_back(toggle);
        addComponent(toggle);
    }
    
    vector<string> options = {ofToString(buffer_sizes[0]), ofToString(buffer_sizes[1]), ofToString(buffer_sizes[2]), ofToString(buffer_sizes[3])};
    
    gBufferSize = GuiFactory::createDropDown(BUFFER_SIZE_LABEL, options, this, &MetersMenuModal::onBufferSizeDropdownEvent);
    addComponent(gBufferSize);
}

void MetersMenuModal::display(int height){
    setHeight(height);
    show();
}

void MetersMenuModal::updateTogglesFromEnabledAlgorithms(){
    auto availableAlgorithms = ofxaa::allAvailableAlgorithmTypes;
    if (_algorithmToggles.size() != availableAlgorithms.size()){
        return;
    }
    
    for (int i=0; i<_algorithmToggles.size(); i++){
        bool enabled = isAlgorithmEnabled(availableAlgorithms[i]);
        _algorithmToggles[i]->setEnabled(enabled);
    }
}

void MetersMenuModal::updateEnabledAlgorithmsFromToggles(){
    auto availableAlgorithms = ofxaa::allAvailableAlgorithmTypes;
    if (_algorithmToggles.size() != availableAlgorithms.size()){
        return;
    }
    enabledAlgorithms.clear();
    for (int i=0; i<_algorithmToggles.size(); i++){
        if (_algorithmToggles[i]->getEnabled()) {
            enabledAlgorithms.push_back(availableAlgorithms[i]);
        }
    }
}

bool MetersMenuModal::isAlgorithmEnabled(ofxAAAlgorithmType algorithmType){
    for(auto enabledType : enabledAlgorithms){
        if (enabledType == algorithmType) {
            return true;
        }
    }
    return false;
}

void MetersMenuModal::applyConfiguration(){
    _metersPanelPtr->updateMetersViewSettings();
    if (selectedBufferSize != -1){
        _metersPanelPtr->setBufferSize(selectedBufferSize);
    }
    updateEnabledAlgorithmsFromToggles();
    _metersPanelPtr->setEnabledAlgorithms(enabledAlgorithms);
}

bool MetersMenuModal::getFocused(){
    return false;
}
void MetersMenuModal::onToggleEvent(ofxDatGuiButtonEvent e){
    //TODO: Implement?
}
void MetersMenuModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
 
    applyConfiguration();
    hide();
}

void MetersMenuModal::onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e){
    selectedBufferSize = buffer_sizes[e.child];
}

void MetersMenuModal::loadStateIntoSettings(MetersPanelSettings* settings){
//    if (selectedBufferSize != -1){
//        settings->bufferSize = selectedBufferSize;
//    }
    
}

void MetersMenuModal::setStateFromSettings(MetersPanelSettings& settings){
    enabledAlgorithms = settings.enabledAlgorithmTypes;
    updateTogglesFromEnabledAlgorithms();
    
    auto index = distance(buffer_sizes.begin(), find(buffer_sizes.begin(), buffer_sizes.end(), settings.bufferSize));
    if (index < buffer_sizes.size()){
        gBufferSize->select(index);
    }
    
}


