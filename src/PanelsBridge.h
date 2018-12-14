//
//  MetersTimelineBridge.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 07/12/2018.
//

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"
#include "MainPanel.h"

class PanelsBridge {
public:
    static PanelsBridge& getInstance(){
        static PanelsBridge instance;
        return instance;
    }
    
    void onsetFired(int & panelId);
    
    bool getIsTimelinePlaying(){
        if (timelinePanelPtr != NULL){return timelinePanelPtr->isPlaying();}
        else {return false;}
    }
    
    void setMainPanelPtr(MainPanel* panelPtr){
        mainPanelPtr = panelPtr;
    }
    void setTimelinePanelPtr(TimelinePanel* panelPtr){
        timelinePanelPtr = panelPtr;
    }
    void setMetersPanelPtr(MetersPanel* panelPtr){
        metersPanelPtr = panelPtr;
    }
    
private:
    PanelsBridge();
    PanelsBridge(PanelsBridge const& copy);
    PanelsBridge& operator=(PanelsBridge const& copy);
    
    MainPanel* mainPanelPtr;
    MetersPanel* metersPanelPtr;
    TimelinePanel* timelinePanelPtr;
};
