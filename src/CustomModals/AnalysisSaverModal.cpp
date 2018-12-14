//
//  AnalysisSaverModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 14/12/2018.
//

#include "AnalysisSaverModal.h"
#include "MainPanel.h"

AnalysisSaverModal::AnalysisSaverModal(MainPanel* mainPanel_ptr){
    _mainPanelPtr = mainPanel_ptr;
    setTitle("RENDER ANALYSIS");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &AnalysisSaverModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
    //TODO: Add message
    
}

void AnalysisSaverModal::display(int height){
    setHeight(height);
    show();
}

void AnalysisSaverModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
    _mainPanelPtr->renderAnalysis();
    hide();
}
