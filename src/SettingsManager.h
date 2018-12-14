//
//  SettingsManager.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 05/12/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "Settings.h"

#include "MainPanel.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"

#define MAIN_PANEL_TAG "MAIN-PANEL"
#define FRAMERATE_TAG "FRAMERATE"
#define BUFFER_SIZE_TAG "BUFFER-SIZE"
#define BPM_TAG "BPM"
#define PORT_TAG "PORT"
#define HOST_TAG "HOST"
#define SEND_OSC_TAG "SEND-OSC"

#define TIMELINE_PANEL_TAG "TIMELINE-PANEL"
#define VOLUME_TAG "VOLUME"
#define LOOP_TAG "LOOP"
#define BPM_GRID_TAG "BPM-GRID"
#define SNAP_TAG "SNAP-BPM"
#define FRAMEBASED_TAG "FRAMEBASED"
#define TRACKS_TAG "TRACKS"
#define TRACKS_NUM_TAG "TRACKS-NUM"
#define TRACK_N_TAG "TRACK-"
#define NAME_TAG "NAME"
#define TYPE_TAG "TYPE"
#define MARKERS_TAG "MARKERS"
#define MARKERS_NUM_TAG "MARKERS-NUM"
#define MARKER_N_TAG "MARKER-"

#define METERS_PANEL_TAG "METERS-PANEL"
#define ENABLED_ALGORITHMS_TAG "ENABLED-ALGORITHMS"
#define ENABLED_NUM_TAG "ENABLED-NUM"
#define TYPE_N_TAG "TYPE-"
#define CHANNELS_TAG "CHANNELS"
#define CHANNELS_NUM_TAG "CHANNELS-NUM"
#define CHANNEL_N_TAG "CHANNEL-"
#define METERS_NUM_TAG "METERS-NUM"
#define METER_N_TAG "METER-"
#define STATE_TAG "STATE"
#define SMOOTH_TAG "SMOOTH"
#define MAX_EST_VALUE_TAG "MAX-EST-VALUE"
#define ALPHA_TAG "ALPHA"
#define SILENCE_THRESHOLD_TAG "SILENCE-THRESHOLD"
#define TIME_THRESHOLD_TAG "TIME-THRESHOLD"
#define TYPE_TAG "TYPE"

class SettingsManager {
    
public:
    static SettingsManager& getInstance(){
        // The only instance
        // Guaranteed to be lazy initialized
        // Guaranteed that it will be destroyed correctly
        static SettingsManager instance;
        return instance;
    }
    
    void saveSettings();
    void loadSettings();
    
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
    // Private Constructor
    SettingsManager();
    // Stop the compiler generating methods of copy the object
    SettingsManager(SettingsManager const& copy);            // Not Implemented
    SettingsManager& operator=(SettingsManager const& copy); // Not Implemented
    
    void saveSettingsToFile();
    void loadSettingsFromFile();
    
    void updateCurrentSettingsFromPanels();
    void loadSettingsIntoPanels();
    void resetPanelSettings();
    
    void addMainPanelSettingsToXml();
    void loadMainPanelSettingsFromXml();
    void addTimelinePanelSettingsToXml();
    void loadTimelinePanelSettingsFromXml();
    void addMetersPanelSettingsToXml();
    void loadMetersPanelSettingsFromXml();
    
    
    
    ofxXmlSettings xml;
    
    bool bXmlSettingsLoaded;
    
    MainPanel* mainPanelPtr;
    MetersPanel* metersPanelPtr;
    TimelinePanel* timelinePanelPtr;
    
    MainPanelSettings mainPanelSettings;
    TimelinePanelSettings timelinePanelSettings;
    MetersPanelSettings metersPanelSettings;
};
