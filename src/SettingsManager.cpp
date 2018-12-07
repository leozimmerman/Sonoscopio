//
//  SettingsManager.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 05/12/2018.
//

#include "SettingsManager.h"
#include "ofxAAUtils.h"
#include "FileManager.h"

SettingsManager::SettingsManager(){
    rootDir = "";
}
void SettingsManager::saveSettings(){
    updateCurrentSettingsFromPanels();
    
    xml.clear();
    addMainPanelSettingsToXml();
    addTimelinePanelSettingsToXml();
    addMetersPanelSettingsToXml();
    saveSettingsToFile();
    
}

void SettingsManager::loadSettings(){
    loadSettingsFromFile();
    loadSettingsIntoPanels();
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
    
    if( xml.loadFile(filename) ){
        ofLogVerbose()<<"SettingsManager: "<< filename <<" loaded.";
    }else{
        ofLogError()<< "SettingsManager: unable to load " << filename ;
        //TODO: Show error modal
        return;
    }
    
    loadMainPanelSettingsFromXml();
    loadTimelinePanelSettingsFromXml();
    loadMetersPanelSettingsFromXml();
}

void SettingsManager::saveSettingsToFile(){
    string filename = FileManager::getInstance().getSettingsFileName();
    xml.save(filename);
}

#pragma mark - Load Settings from XML

void SettingsManager::loadMainPanelSettingsFromXml(){
    const std::string frTag = std::string(MAIN_PANEL_TAG) + ":" + FRAMERATE_TAG;
    const std::string bsTag = std::string(MAIN_PANEL_TAG) + ":" + BUFFER_SIZE_TAG;
    const std::string bpmTag = std::string(MAIN_PANEL_TAG) + ":" + BPM_TAG;
    const std::string hostTag = std::string(MAIN_PANEL_TAG) + ":" + HOST_TAG;
    const std::string portTag = std::string(MAIN_PANEL_TAG) + ":" + PORT_TAG;
    const std::string sendTag = std::string(MAIN_PANEL_TAG) + ":" + SEND_OSC_TAG;
    
    mainPanelSettings.frameRate = xml.getValue(frTag, 0);
    mainPanelSettings.bufferSize = xml.getValue(bsTag, 0);
    mainPanelSettings.bpm = xml.getValue(bpmTag, 0.0);
    mainPanelSettings.osc.host = xml.getValue(hostTag, "");
    mainPanelSettings.osc.port = xml.getValue(portTag, 0);
    mainPanelSettings.osc.bSend = xml.getValue(sendTag, 0) > 0;
    
}

void SettingsManager::loadTimelinePanelSettingsFromXml(){
    const std::string volTag = std::string(TIMELINE_PANEL_TAG) + ":" + VOLUME_TAG;
    const std::string loopTag = std::string(TIMELINE_PANEL_TAG) + ":" + LOOP_TAG;
    const std::string bpmGridTag = std::string(TIMELINE_PANEL_TAG) + ":" + BPM_GRID_TAG;
    const std::string snapTag = std::string(TIMELINE_PANEL_TAG) + ":" + SNAP_TAG;
    const std::string framebasedTag = std::string(TIMELINE_PANEL_TAG) + ":" + FRAMEBASED_TAG;
       const std::string markersNumTag = std::string(TIMELINE_PANEL_TAG) + ":" + MARKERS_TAG + ":" + MARKERS_NUM_TAG;
       const std::string tracksNumTag = std::string(TIMELINE_PANEL_TAG) + ":" + TRACKS_TAG + ":" + TRACKS_NUM_TAG;
    
    timelinePanelSettings.volume = xml.getValue(volTag, 0.0);
    timelinePanelSettings.bLoop = xml.getValue(loopTag, 0) > 0;
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
    
    const std::string enabledNumTag = std::string(METERS_PANEL_TAG) + ":" + ENABLED_ALGORITHMS_TAG + ":" + ENABLED_NUM_TAG;
    int enabledNum = xml.getValue(enabledNumTag, 0);
    
    metersPanelSettings.enabledAlgorithmTypes.clear();
    for (int i=0; i<enabledNum; i++){
        const std::string typeTag = std::string(METERS_PANEL_TAG) + ":" + ENABLED_ALGORITHMS_TAG + ":" + TYPE_N_TAG + ofToString(i);
        string stringType = xml.getValue(typeTag, "");
        auto algorithm = ofxaa::stringToAlgorithmType(stringType);
        metersPanelSettings.enabledAlgorithmTypes.push_back(algorithm);
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
            const std::string smoothTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + SMOOTH_TAG;
            const std::string alphaTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + ALPHA_TAG;
            const std::string silenceTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + SILENCE_THRESHOLD_TAG;
            const std::string timeTag = std::string(METERS_PANEL_TAG) + ":" + CHANNELS_TAG + ":" + CHANNEL_N_TAG + ofToString(i) + ":" + METER_N_TAG + ofToString(j) + ":" + TIME_THRESHOLD_TAG;

            MeterSettings ms;
            ms.type = xml.getValue(typeTag, "");
            ms.bState = xml.getValue(stateTag, 0) > 0;
            ms.smooth = xml.getValue(smoothTag, 0.0);
            ms.alpha = xml.getValue(alphaTag, 0.0);
            ms.silenceTreshold = xml.getValue(silenceTag, 0.0);
            ms.timeTreshold = xml.getValue(timeTag, 0.0);
            channelSettings.meters.push_back(ms);
        }
        metersPanelSettings.channelMeters.push_back(channelSettings);
    }
    
}

#pragma mark - Add Settings to XML

void SettingsManager::addMainPanelSettingsToXml(){
    xml.addTag(MAIN_PANEL_TAG);
    xml.pushTag(MAIN_PANEL_TAG);
    
    xml.addValue(FRAMERATE_TAG, mainPanelSettings.frameRate);
    xml.addValue(BUFFER_SIZE_TAG, mainPanelSettings.bufferSize);
    xml.addValue(BPM_TAG,  mainPanelSettings.bpm);
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
    
    xml.addTag(ENABLED_ALGORITHMS_TAG);
    xml.pushTag(ENABLED_ALGORITHMS_TAG);
    xml.addValue(ENABLED_NUM_TAG, int(metersPanelSettings.enabledAlgorithmTypes.size()));
    for(int i=0; i<metersPanelSettings.enabledAlgorithmTypes.size(); i++){
        const std::string typeTag = TYPE_N_TAG + ofToString(i);
        string stringType = ofxaa::algorithmTypeToString(metersPanelSettings.enabledAlgorithmTypes[i]);
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
            xml.addValue(TYPE_TAG, metersPanelSettings.channelMeters[i].meters[j].type);
            if (metersPanelSettings.channelMeters[i].meters[j].type == "ONSETS"){
                xml.addValue(ALPHA_TAG, metersPanelSettings.channelMeters[i].meters[j].alpha);
                xml.addValue(SILENCE_THRESHOLD_TAG, metersPanelSettings.channelMeters[i].meters[j].silenceTreshold);
                xml.addValue(TIME_THRESHOLD_TAG, metersPanelSettings.channelMeters[i].meters[j].timeTreshold);
            } else {
                xml.addValue(SMOOTH_TAG, metersPanelSettings.channelMeters[i].meters[j].smooth);
            }
            xml.popTag();
        }
        xml.popTag();
    }
    xml.popTag();//CHANNELS_TAG
    
    xml.popTag();
    
}
