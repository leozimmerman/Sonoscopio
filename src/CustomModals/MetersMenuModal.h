//
//  MetersMenuModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 30/11/2018.
//

#pragma once

#include "ofMain.h"
#include "MetersView.h"
#include "ofxModal.h"

class MetersMenuModal : public ofxModalWindow {
public:
    
    MetersMenuModal(MetersView* mtrView_ptr);
    
    void setMainAppPtr(ofBaseApp* appPtr);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    //void onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e);
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiButtonEvent e);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
    
    
private:
    vector<ofxDatGuiToggle*> _algorithmToggles;
    MetersView* _metersViewPtr;
    
    std::function<void(MetersView*, vector<ofxAAAlgorithmType>&)> callback_setEnabledAlgorithms = &MetersView::setEnabledAlgorithms;
    
};
