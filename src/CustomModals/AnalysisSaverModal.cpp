//
//  AnalysisSaverModal.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 14/12/2018.
//

#include "AnalysisSaverModal.h"
#include "MainPanel.h"
#include "AnalysisDataSaver.h"

AnalysisSaverModal::AnalysisSaverModal(MainPanel* mainPanel_ptr){
    _mainPanelPtr = mainPanel_ptr;
    setTitle("RENDER ANALYSIS");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &AnalysisSaverModal::onApplyButtonEvent);
    
    addButton("CANCEL");
}

void AnalysisSaverModal::display(int height){
    string msg = AnalysisDataSaver::getInstance().getRenderInfoString();
    setMessage(msg);
    setHeight(height);
    show();
}

void AnalysisSaverModal::onApplyButtonEvent(ofxDatGuiButtonEvent e){
    _mainPanelPtr->renderAnalysis();
    hide();
}
