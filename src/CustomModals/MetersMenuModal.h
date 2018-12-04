//
//  MetersMenuModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 30/11/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"

class MetersPanel;

class MetersMenuModal : public ofxModalWindow {
public:
    
    MetersMenuModal(MetersPanel* metersPanel_ptr);
    
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiButtonEvent e);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
private:
    vector<ofxDatGuiToggle*> _algorithmToggles;
    MetersPanel* _metersPanelPtr;
};
