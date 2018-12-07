//
//  MeterModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 07/12/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"

#define FREEZE_LABEL "FREEZE METER"
#define MAX_LABEL "MAX ESTIMATED VALUE"

class MeterView;

class MeterModal : public ofxModalWindow {
    
public:
    
    MeterModal(MeterView* meter_ptr);
    
    void display(int height);
    void applySettings();
    bool getFocused();
    
    //void loadStateIntoSettings(MainPanelSettings* settings);
    //void setStateFromSettings(MainPanelSettings& settings);
    
private:
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    
    ofxDatGuiTextInput* gMaxValue;
    
    MeterView* _meter_ptr;
    
    /*
     TODO: Mover a Meters Modal
     ofxDatGuiTextInput* gMaxFreq;
     ofxDatGuiTextInput* gMaxHfc;
     ofxDatGuiTextInput* gMaxCentroid;
     ofxDatGuiTextInput* gMaxSpecComp;
     ofxDatGuiTextInput* gMaxRollOff;
     ofxDatGuiTextInput* gMaxOddEven;
     */
};
