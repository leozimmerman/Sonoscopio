//
//  MetersMenuModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 30/11/2018.
//

#include "MetersMenuModal.h"
#include "AvailableAlgorithms.h"
#include "StringUtils.h"
#include "MetersPanel.h"
#include "GuiFactory.h"

MetersMenuModal::MetersMenuModal(MetersPanel* metersPanel_ptr){
    _metersPanelPtr = metersPanel_ptr;
    setTitle("METERS MENU");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MetersMenuModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    selectedBufferSize = -1;
  
    
    auto availableValues = sonoscopio::allAvailableValueTypes;
    for (int i=0; i<availableValues.size(); i++){
        auto label = utils::valueTypeToString(availableValues[i]);
        ofxDatGuiToggle* toggle = GuiFactory::createToggle(label, TRUE, this, &MetersMenuModal::onToggleEvent);
        _valuesToggles.push_back(toggle);
        if (availableValues[i] == MEL_BANDS_KURTOSIS){
            cout<<"Hola"<<endl;
        }
        addComponentInGrid(toggle);
    }
    
    auto availableBinsValues = sonoscopio::allAvailableBinsValueTypes;
    for (int i=0; i<availableBinsValues.size(); i++){
        auto label = utils::binsValueTypeToString(availableBinsValues[i]);
        ofxDatGuiToggle* toggle = GuiFactory::createToggle(label, TRUE, this, &MetersMenuModal::onToggleEvent);
        _binsValuesToggles.push_back(toggle);
        addComponentInGrid(toggle);
    }
    
    vector<string> options = {ofToString(buffer_sizes[0]), ofToString(buffer_sizes[1]), ofToString(buffer_sizes[2]), ofToString(buffer_sizes[3])};
    
    gBufferSize = GuiFactory::createDropDown(BUFFER_SIZE_LABEL, options, this, &MetersMenuModal::onBufferSizeDropdownEvent);
    addComponentInGrid(gBufferSize);
}

void MetersMenuModal::addComponentInGrid(ofxDatGuiComponent* component){
    int togglesWidth = getWidth()/4;
    int togglesPerColumn = 19;
    int compHeight = 26;
    int yOffset = 15;
    
    int n = (int) mModalComponents.size();
    
    int mod = n % togglesPerColumn;
    int xmod = n/togglesPerColumn;
    int x = togglesWidth * xmod;
    int y = mod * compHeight + yOffset;
    addComponent(component, x, y, togglesWidth);
}

void MetersMenuModal::display(int height){
    setHeight(height);
    show();
}

void MetersMenuModal::updateTogglesFromEnabledAlgorithms(){
    auto availableValues = sonoscopio::allAvailableValueTypes;
    if (_valuesToggles.size() != availableValues.size()){
        return;
    }
    for (int i=0; i<_valuesToggles.size(); i++){
        bool enabled = isAlgorithmEnabled(availableValues[i]);
        _valuesToggles[i]->setEnabled(enabled);
    }
    
    auto availableBinsValues = sonoscopio::allAvailableBinsValueTypes;
    if (_binsValuesToggles.size() != availableBinsValues.size()){
        return;
    }
    for (int i=0; i<_binsValuesToggles.size(); i++){
        bool enabled = isAlgorithmEnabled(availableBinsValues[i]);
        _binsValuesToggles[i]->setEnabled(enabled);
    }
}

void MetersMenuModal::updateEnabledAlgorithmsFromToggles(){
    auto availableValues = sonoscopio::allAvailableValueTypes;
    if (_valuesToggles.size() != availableValues.size()){
        return;
    }
    _enabledValueTypes.clear();
    for (int i=0; i<_valuesToggles.size(); i++){
        if (_valuesToggles[i]->getEnabled()) {
            _enabledValueTypes.push_back(availableValues[i]);
        }
    }
    
    auto availableBinsValues = sonoscopio::allAvailableBinsValueTypes;
    if (_binsValuesToggles.size() != availableBinsValues.size()){
        return;
    }
    _enabledBinsValueTypes.clear();
    for (int i=0; i<_binsValuesToggles.size(); i++){
        if (_binsValuesToggles[i]->getEnabled()) {
            _enabledBinsValueTypes.push_back(availableBinsValues[i]);
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
//    cout<<"TOggle"<<endl;
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


