//
//  MetersTimelineBridge.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 07/12/2018.
//

#include "PanelsBridge.h"

PanelsBridge::PanelsBridge(){
    ofAddListener(OnsetMeterView::onsetEventGlobal, this, &PanelsBridge::onsetFired);
}

void PanelsBridge::onsetFired(int & panelId){
    timelinePanelPtr->addKeyframeInFocusedTrack();
}

void PanelsBridge::freezeMeter(MeterView& meter){
    cout<<"Bridge: Freezing meter..."<<endl;
}
