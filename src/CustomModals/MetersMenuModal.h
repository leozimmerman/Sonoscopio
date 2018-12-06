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

class MetersPanel;

class MetersMenuModal : public ofxModalWindow {
public:
    
    MetersMenuModal(MetersPanel* metersPanel_ptr);
    
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiButtonEvent e);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
    void loadStateIntoSettings(MetersPanelSettings* settings);
    void setStateFromSettings(MetersPanelSettings& settings);
    
    void updateTogglesFromEnabledAlgorithms();
    void updateEnabledAlgorithmsFromToggles();
    
    bool isAlgorithmEnabled(ofxAAAlgorithmType algorithmType);
    
private:
    vector<ofxDatGuiToggle*> _algorithmToggles;
    vector<ofxAAAlgorithmType> enabledAlgorithms;
    MetersPanel* _metersPanelPtr;
};
