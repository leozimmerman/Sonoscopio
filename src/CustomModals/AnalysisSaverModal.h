//
//  AnalysisSaverModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 14/12/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"
#include "Settings.h"

class MainPanel;

class AnalysisSaverModal : public ofxModalWindow {
public:
    
    AnalysisSaverModal(MainPanel* mainPanel_ptr);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
private:
    
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    MainPanel* _mainPanelPtr;
    
};
