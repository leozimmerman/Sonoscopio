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
    
    auto availableValues = ofxaa::allAvailableValueTypes;
    for (int i=0; i<availableValues.size(); i++){
        auto label = ofxaa::valueTypeToString(availableValues[i]);
        ofxDatGuiToggle* toggle = GuiFactory::createToggle(label, TRUE, this, &MetersMenuModal::onToggleEvent);
        _valuesToggles.push_back(toggle);
        addComponent(toggle);
    }
    
    auto availableBinsValues = ofxaa::allAvailableBinsValueTypes;
    for (int i=0; i<availableBinsValues.size(); i++){
        auto label = ofxaa::binsValueTypeToString(availableBinsValues[i]);
        ofxDatGuiToggle* toggle = GuiFactory::createToggle(label, TRUE, this, &MetersMenuModal::onToggleEvent);
        _binsValuesToggles.push_back(toggle);
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
    auto availableValues = ofxaa::allAvailableValueTypes;
    if (_valuesToggles.size() != availableValues.size()){
        return;
    }
    for (int i=0; i<_valuesToggles.size(); i++){
        bool enabled = isAlgorithmEnabled(availableValues[i]);
        _valuesToggles[i]->setEnabled(enabled);
    }
    
    auto availableBinsValues = ofxaa::allAvailableBinsValueTypes;
    if (_binsValuesToggles.size() != availableValues.size()){
        return;
    }
    for (int i=0; i<_binsValuesToggles.size(); i++){
        bool enabled = isAlgorithmEnabled(availableBinsValues[i]);
        _binsValuesToggles[i]->setEnabled(enabled);
    }
}

void MetersMenuModal::updateEnabledAlgorithmsFromToggles(){
    auto availableValues = ofxaa::allAvailableValueTypes;
    if (_valuesToggles.size() != availableValues.size()){
        return;
    }
    _enabledValueTypes.clear();
    for (int i=0; i<_valuesToggles.size(); i++){
        if (_valuesToggles[i]->getEnabled()) {
            _enabledValueTypes.push_back(availableValues[i]);
        }
    }
    
    auto availableBinsValues = ofxaa::allAvailableBinsValueTypes;
    if (_binsValuesToggles.size() != availableValues.size()){
        return;
    }
    _enabledBinsValueTypes.clear();
    for (int i=0; i<_binsValuesToggles.size(); i++){
        if (_binsValuesToggles[i]->getEnabled()) {
            _enabledValueTypes.push_back(availableValues[i]);
        }
    }
}

bool MetersMenuModal::isAlgorithmEnabled(ofxAAValue valueType){
    for(auto enabledType : _enabledValueTypes){
        if (enabledType == valueType) {
            return true;
        }
    }
    return false;
}

bool MetersMenuModal::isAlgorithmEnabled(ofxAABinsValue valueType){
    for(auto enabledType : _enabledBinsValueTypes){
        if (enabledType == valueType) {
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
    _metersPanelPtr->setEnabledAlgorithms(_enabledValueTypes, _enabledBinsValueTypes);
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
    _enabledValueTypes = settings.enabledValueTypes;
    _enabledBinsValueTypes = settings.enabledBinsValueTypes;
    updateTogglesFromEnabledAlgorithms();
    
    auto index = distance(buffer_sizes.begin(), find(buffer_sizes.begin(), buffer_sizes.end(), settings.bufferSize));
    if (index < buffer_sizes.size()){
        gBufferSize->select(int(index));
    }
    
}


