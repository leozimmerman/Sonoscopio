//
//  MetersMenuModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 30/11/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"
#include "Settings.h"

#define BUFFER_SIZE_LABEL "BUFFER SIZE"

class MetersPanel;

class MetersMenuModal : public ofxModalWindow {
public:
    
    MetersMenuModal(MetersPanel* metersPanel_ptr);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
    void loadStateIntoSettings(MetersPanelSettings* settings);
    void setStateFromSettings(MetersPanelSettings& settings);
    
    void updateTogglesFromEnabledAlgorithms();
    void updateEnabledAlgorithmsFromToggles();
    
    bool isAlgorithmEnabled(ofxAAValue algorithmType);
    bool isAlgorithmEnabled(ofxAABinsValue algorithmType);
    
private:
    void addComponentInGrid(ofxDatGuiComponent* component);
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiButtonEvent e);
    void onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e);
    
    vector<ofxDatGuiToggle*> _valuesToggles;
    vector<ofxDatGuiToggle*> _binsValuesToggles;
    ofxDatGuiDropdown* gBufferSize;
    
    int selectedBufferSize;
    const vector<int> buffer_sizes = {256, 512, 1024, 2048};
    
    vector<ofxAAValue> _enabledValueTypes;
    vector<ofxAABinsValue> _enabledBinsValueTypes;
    
    MetersPanel* _metersPanelPtr;
};
