//
//  SettingsManager.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 05/12/2018.
//

#include "SettingsManager.h"
#include "StringUtils.h"
#include "FileManager.h"

SettingsManager::SettingsManager(){
    bXmlSettingsLoaded = false;
}

void SettingsManager::saveSettings(){
    updateCurrentSettingsFromPanels();
    xml.clear();
    addVersionSettingsToXml();
    addMainPanelSettingsToXml();
    addTimelinePanelSettingsToXml();
    addMetersPanelSettingsToXml();
    saveSettingsToFile();
}

void SettingsManager::loadSettings(){
    loadSettingsFromFile();
    if (bXmlSettingsLoaded){
        loadSettingsIntoPanels();
    } else {
        resetPanelSettings();
    }
}

void SettingsManager::updateCurrentSettingsFromPanels(){
    if (mainPanelPtr != NULL) {
        mainPanelPtr->updateCurrentSettings();
        mainPanelSettings = *mainPanelPtr->getCurrentSettingsPtr();
    }
    if (timelinePanelPtr != NULL){
        timelinePanelPtr->updateCurrentSettings();
        timelinePanelSettings = *timelinePanelPtr->getCurrentSettingsPtr();
    }
    if (metersPanelPtr != NULL){
        metersPanelPtr->updateCurrentSettings();
        metersPanelSettings = *metersPanelPtr->getCurrentSettingsPtr();
    }
}

void SettingsManager::resetPanelSettings(){
    if (mainPanelPtr != NULL) {
        mainPanelPtr->resetSettings();
    }
    if (timelinePanelPtr != NULL){
        timelinePanelPtr->resetSettings();
    }
    if (metersPanelPtr != NULL){
        metersPanelPtr->resetSettings();
    }
}

void SettingsManager::loadSettingsIntoPanels(){
    if (mainPanelPtr != NULL) {
        mainPanelPtr->loadSettings(mainPanelSettings);
    }
    if (timelinePanelPtr != NULL){
        timelinePanelPtr->loadSettings(timelinePanelSettings);
    }
    if (metersPanelPtr != NULL){
        metersPanelPtr->loadSettings(metersPanelSettings);
    }
}

void SettingsManager::loadSettingsFromFile(){
    xml.clear();
    string filename = FileManager::getInstance().getSettingsFileName();
    if(xml.loadFile(filename)){
        //TODO: Check version matches
        loadMainPanelSettingsFromXml();
        loadTimelinePanelSettingsFromXml();
        loadMetersPanelSettingsFromXml();
        bXmlSettingsLoaded = true;
    } else {
        bXmlSettingsLoaded = false;
    }
}

void SettingsManager::saveSettingsToFile(){
    if (timelinePanelPtr != NULL){
        timelinePanelPtr->saveTracksDataToFolder();
    }
    string filename = FileManager::getInstance().getSettingsFileName();
    xml.save(filename);
}

#pragma mark - Load Settings from XML

void SettingsManager::loadMainPanelSettingsFromXml(){
    const std::string frTag = std::string(MAIN_PANEL_TAG) + ":" + FRAMERATE_TAG;
    const std::string hostTag = std::string(MAIN_PANEL_TAG) + ":" + HOST_TAG;
    const std::string portTag = std::string(MAIN_PANEL_TAG) + ":" + PORT_TAG;
    const std::string sendTag = std::string(MAIN_PANEL_TAG) + ":" + SEND_OSC_TAG;
    
    mainPanelSettings.frameRate = xml.getValue(frTag, 0);
    mainPanelSettings.osc.host = xml.getValue(hostTag, "");
    mainPanelSettings.osc.port = xml.getValue(portTag, 0);
    mainPanelSettings.osc.bSend = xml.getValue(sendTag, 0) > 0;
}

void SettingsManager::loadTimelinePanelSettingsFromXml(){
    const std::string volTag = std::string(TIMELINE_PANEL_TAG) + ":" + VOLUME_TAG;
    const std::string loopTag = std::string(TIMELINE_PANEL_TAG) + ":" + LOOP_TAG;
    const std::string bpmTag = std::string(TIMELINE_PANEL_TAG) + ":" + BPM_TAG;
    const std::string bpmGridTag = std::string(TIMELINE_PANEL_TAG) + ":" + BPM_GRID_TAG;
    const std::string snapTag = std::string(TIMELINE_PANEL_TAG) + ":" + SNAP_TAG;
    const std::string framebasedTag = std::string(TIMELINE_PANEL_TAG) + ":" + FRAMEBASED_TAG;
    const std::string markersNumTag = std::string(TIMELINE_PANEL_TAG) + ":" + MARKERS_TAG + ":" + MARKERS_NUM_TAG;
    const std::string tracksNumTag = std::string(TIMELINE_PANEL_TAG) + ":" + TRACKS_TAG + ":" + TRACKS_NUM_TAG;
    
    timelinePanelSettings.volume = xml.getValue(volTag, 0.0);
    timelinePanelSettings.bLoop = xml.getValue(loopTag, 0) > 0;
    timelinePanelSettings.bpm = xml.getValue(bpmTag, 0.0);
    timelinePanelSettings.bBpmGrid = xml.getValue(bpmGridTag, 0) > 0;
    timelinePanelSettings.bSnap= xml.getValue(snapTag, 0) > 0;
    timelinePanelSettings.bFrambased = xml.getValue(framebasedTag, 0) > 0;
    
    int markersNum = xml.getValue(markersNumTag, 0);
    timelinePanelSettings.markers.clear();
    for (int i=0; i<markersNum; i++){
        const std::string markerNTag = std::string(TIMELINE_PANEL_TAG) + ":" + MARKERS_TAG + ":" + MARKER_N_TAG + ofToString(i);
        float marker = xml.getValue(markerNTag, 0.0);
        timelinePanelSettings.markers.push_back(marker);
    }
    
    int tracksNum = xml.getValue(tracksNumTag, 0);
    timelinePanelSettings.tracks.clear();
    for (int i=0; i<tracksNum; i++){
        const std::string nameTag = std::string(TIMELINE_PANEL_TAG) + ":" + TRACKS_TAG + ":" + TRACK_N_TAG + ofToString(i) + ":" + NAME_TAG;
        const std::string typeTag = std::string(TIMELINE_PANEL_TAG) + ":" + TRACKS_TAG + ":" + TRACK_N_TAG + ofToString(i) + ":" + TYPE_TAG;
        
        TLTrackSetting track;
        track.name = xml.getValue(nameTag, "");
        track.type = xml.getValue(typeTag, "");
        timelinePanelSettings.tracks.push_back(track);
    }
}

void SettingsManager::loadMetersPanelSettingsFromXml(){
    
    const std::string bsTag = std::string(METERS_PANEL_TAG) + ":" + BUFFER_SIZE_TAG;

    metersPanelSettings.bufferSize = xml.getValue(bsTag, 0);
    
    const std::string enabledNumTag = std::string(METERS_PANEL_TAG) + ":" + ENABLED_ALGORITHMS_TAG + ":" + ENABLED_VALUES_NUM_TAG;
    int enabledValuesNum = xml.getValue(enabledNumTag, 0);
    const std::string enabledBinNumTag = std::string(METERS_PANEL_TAG) + ":" + ENABLED_ALGORITHMS_TAG + ":" + ENABLED_BINVALUES_NUM_TAG;
    int enabledBinValuesNum = xml.getValue(enabledBinNumTag, 0);
    
    metersPanelSettings.enabledValueTypes.clear();
    metersPanelSettings.enabledBinsValueTypes.clear();
    
    for (int i=0; i<enabledValuesNum; i++){
        const std::string typeTag = std::string(METERS_PANEL_TAG) + ":" + ENABLED_ALGORITHMS_TAG + ":" + VALUE_TYPE_N_TAG + ofToString(i);
        string stringType = xml.getValue(typeTag, "");
        ofxAAValue algorithm = utils::stringToValueType(stringType);
        metersPanelSettings.enabledValueTypes.push_back(algorithm);
    }
    
    for (int i=0; i<enabledBinValuesNum; i++){
        const std::string typeTag = std::string(METERS_PANEL_TAG) + ":" + ENABLED_ALGORITHMS_TAG + ":" + BIN_VALUE_TYPE_N_TAG + ofToString(i);
        string stringType = xml.getValue(typeTag, "");
        ofxAABinsValue algorithm = utils::stringToBinsValueType(stringType);
        metersPanelSettings.enabledBinsValueTypes.push_back(algorithm);
    }
    
    const std::string channelsNumTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNELS_NUM_TAG;
    int channelsNum = xml.getValue(channelsNumTag, 0);
    
    metersPanelSettings.channelMeters.clear();
    for (int i=0; i<channelsNum; i++){
        ChannelMeterSettings channelSettings;
        const std::string metersNumTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METERS_NUM_TAG;
        int metersNum = xml.getValue(metersNumTag, 0);
        
        for (int j=0; j<metersNum; j++){
            const std::string typeTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + TYPE_TAG;
            const std::string stateTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + STATE_TAG;
            const std::string plotTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + PLOT_TAG;
            const std::string smoothTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + SMOOTH_TAG;
            const std::string maxValueTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + MAX_EST_VALUE_TAG;
            const std::string alphaTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + ALPHA_TAG;
            const std::string silenceTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + SILENCE_THRESHOLD_TAG;
            const std::string timeTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + TIME_THRESHOLD_TAG;

            MeterSettings ms;
            ms.type = xml.getValue(typeTag, "");
            ms.bState = xml.getValue(stateTag, 0) > 0;
            ms.bPlotValue = xml.getValue(plotTag, 0) > 0;
            ms.smooth = xml.getValue(smoothTag, 0.0);
            ms.maxEstimatedValue = xml.getValue(maxValueTag, 0.0);
            ms.alpha = xml.getValue(alphaTag, 0.0);
            ms.silenceTreshold = xml.getValue(silenceTag, 0.0);
            ms.timeTreshold = xml.getValue(timeTag, 0.0);
            channelSettings.meters.push_back(ms);
        }
        metersPanelSettings.channelMeters.push_back(channelSettings);
    }
    
}

#pragma mark - Add Settings to XML

void SettingsManager::addVersionSettingsToXml(){
    xml.addTag(SONOSCOPIO_INFO_TAG);
    xml.pushTag(SONOSCOPIO_INFO_TAG);
    xml.addValue(VERSION_TAG, SONOSCOPIO_VERSION);
    xml.popTag();
}

void SettingsManager::addMainPanelSettingsToXml(){
    xml.addTag(MAIN_PANEL_TAG);
    xml.pushTag(MAIN_PANEL_TAG);
    xml.addValue(FRAMERATE_TAG, mainPanelSettings.frameRate);
    xml.addValue(HOST_TAG, mainPanelSettings.osc.host);
    xml.addValue(PORT_TAG, mainPanelSettings.osc.port);
    xml.addValue(SEND_OSC_TAG, mainPanelSettings.osc.bSend);
    xml.popTag();
}

void SettingsManager::addTimelinePanelSettingsToXml(){
    xml.addTag(TIMELINE_PANEL_TAG);
    xml.pushTag(TIMELINE_PANEL_TAG);
    
    xml.addValue(VOLUME_TAG, timelinePanelSettings.volume);
    xml.addValue(LOOP_TAG, timelinePanelSettings.bLoop);
    xml.addValue(BPM_TAG,  timelinePanelSettings.bpm);
    xml.addValue(BPM_GRID_TAG, timelinePanelSettings.bBpmGrid);
    xml.addValue(SNAP_TAG, timelinePanelSettings.bSnap);
    xml.addValue(FRAMEBASED_TAG, timelinePanelSettings.bFrambased);
    
    xml.addTag(TRACKS_TAG);
    xml.pushTag(TRACKS_TAG);
    xml.addValue(TRACKS_NUM_TAG, int(timelinePanelSettings.tracks.size()));
    for (int i=0; i<timelinePanelSettings.tracks.size(); i++){
        std::string trackTag = TRACK_N_TAG + ofToString(i);
        xml.addTag(trackTag);
        xml.pushTag(trackTag);
        xml.addValue(NAME_TAG, timelinePanelSettings.tracks[i].name);
        xml.addValue(TYPE_TAG, timelinePanelSettings.tracks[i].type);
        xml.popTag();
    }
    xml.popTag();
    
    xml.addTag(MARKERS_TAG);
    xml.pushTag(MARKERS_TAG);
    xml.addValue(MARKERS_NUM_TAG, int(timelinePanelSettings.markers.size()));
    for (int i=0; i<timelinePanelSettings.markers.size(); i++){
        std::string markerTag = MARKER_N_TAG + ofToString(i);
        xml.addValue(markerTag, timelinePanelSettings.markers[i]);
    }
    xml.popTag();
    
    xml.popTag();
    
}

void SettingsManager::addMetersPanelSettingsToXml(){
    xml.addTag(METERS_PANEL_TAG);
    xml.pushTag(METERS_PANEL_TAG);
    
    xml.addValue(BUFFER_SIZE_TAG, metersPanelSettings.bufferSize);
    
    xml.addTag(ENABLED_ALGORITHMS_TAG);
    xml.pushTag(ENABLED_ALGORITHMS_TAG);
    
    xml.addValue(ENABLED_VALUES_NUM_TAG, int(metersPanelSettings.enabledValueTypes.size()));
    for(int i=0; i<metersPanelSettings.enabledValueTypes.size(); i++){
        const std::string typeTag = VALUE_TYPE_N_TAG + ofToString(i);
        string stringType = utils::valueTypeToString(metersPanelSettings.enabledValueTypes[i]);
        xml.addValue(typeTag, stringType);
    }
    
    xml.addValue(ENABLED_BINVALUES_NUM_TAG, int(metersPanelSettings.enabledBinsValueTypes.size()));
    for(int i=0; i<metersPanelSettings.enabledBinsValueTypes.size(); i++){
        const std::string typeTag = BIN_VALUE_TYPE_N_TAG + ofToString(i);
        string stringType = utils::binsValueTypeToString(metersPanelSettings.enabledBinsValueTypes[i]);
        xml.addValue(typeTag, stringType);
    }
    xml.popTag();
    
    xml.addTag(CHANNELS_TAG);
    xml.pushTag(CHANNELS_TAG);
    xml.addValue(CHANNELS_NUM_TAG, int(metersPanelSettings.channelMeters.size()));
    for (int i=0; i<metersPanelSettings.channelMeters.size(); i++){
        std::string channelTag = CHANNEL_N_TAG + ofToString(i);
        xml.addTag(channelTag);
        xml.pushTag(channelTag);
        xml.addValue(METERS_NUM_TAG, int(metersPanelSettings.channelMeters[i].meters.size()));
        for (int j=0; j<metersPanelSettings.channelMeters[i].meters.size(); j++){
            std::string meterTag = METER_N_TAG + ofToString(j);
            xml.addTag(meterTag);
            xml.pushTag(meterTag);
            
            xml.addValue(STATE_TAG, metersPanelSettings.channelMeters[i].meters[j].bState);
            xml.addValue(PLOT_TAG, metersPanelSettings.channelMeters[i].meters[j].bPlotValue);
            xml.addValue(TYPE_TAG, metersPanelSettings.channelMeters[i].meters[j].type);
            if (metersPanelSettings.channelMeters[i].meters[j].type == "ONSETS"){
                xml.addValue(ALPHA_TAG, metersPanelSettings.channelMeters[i].meters[j].alpha);
                xml.addValue(SILENCE_THRESHOLD_TAG, metersPanelSettings.channelMeters[i].meters[j].silenceTreshold);
                xml.addValue(TIME_THRESHOLD_TAG, metersPanelSettings.channelMeters[i].meters[j].timeTreshold);
            } else {
                xml.addValue(SMOOTH_TAG, metersPanelSettings.channelMeters[i].meters[j].smooth);
                xml.addValue(MAX_EST_VALUE_TAG, metersPanelSettings.channelMeters[i].meters[j].maxEstimatedValue);
            }
            xml.popTag();
        }
        xml.popTag();
    }
    xml.popTag();//CHANNELS_TAG
    
    xml.popTag();
    
}
