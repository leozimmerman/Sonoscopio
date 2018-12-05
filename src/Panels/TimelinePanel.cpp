/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * Sonoscopio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#include "TimelinePanel.h"
#include "SettingsManager.h"
#include "ofApp.h"


#pragma mark - Core funcs

//-------------------------------------------------

void TimelinePanel::setup(int x, int y, int w, int h, string audiofile){
    
    BasePanel::setup(x, y, w, h);
    
    guiView.setup(x, y, w, GUI_COMP_HEIGHT * 3, &timelineView);
    timelineView.setup(x, guiView.maxY(), w, h - guiView.getHeight(), audiofile);
    
    SettingsManager::getInstance().setTimelinePanelPtr(this);
}
//-------------------------------------------------
void TimelinePanel::update(){
    timelineView.update();
    guiView.update();
}
//-------------------------------------------------
void TimelinePanel::draw(){
    if (!View::mustDrawNewFrame()){
        View::drawLoadedTexture();
        return;
    }
    
    if (_isHidden){ return; }
    View::draw();
    TS_START("timeline");
    timelineView.draw();
    TS_STOP("timeline");
    
    TS_START("gui-tl");
    guiView.draw();
    TS_STOP("gui-tl");
    
    View::loadViewInTexture();
    
}
//--------------------------------------------------------------
void TimelinePanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, GUI_COMP_HEIGHT);
    timelineView.resize(x, guiView.maxY(), w, h - guiView.getHeight());
    
}
//--------------------------------------------------------------
void TimelinePanel::keyPressed(int key){
    timelineView.keyPressed(key);
}

//--------------------------------------------------------------
bool TimelinePanel::getFocused(){
    return guiView.getFocused();
}
void TimelinePanel::exit(){}
//--------------------------------------------------------------
#pragma mark - Settings
void TimelinePanel::loadSettings(){
    //TODO: Update current settings
    guiView.setStateFromSettings(currentSettings);
    
    timelineView.clearMarkers();
    for (auto m : currentSettings.markers){
        timelineView.addMarkerAtTime(m);
    }
    
    auto tracksPage = timelineView.timeline.getPage(PAGE_TRACKS_NAME);
    for (auto t : currentSettings.tracks){
        if(t.name != "DEFAULT" && tracksPage->getTrack(t.name)==NULL) {
            timelineView.addTrackWithStringType(t.type, t.name);
        }
    }
    loadTimelineTracksFromFolder();///?
    timelineView.updateHeight();
}

//--------------------------------------------------------------
void TimelinePanel::updateCurrentSettings(){
    guiView.loadStateIntoSettings(&currentSettings);
    auto tracksPage = timelineView.timeline.getPage(PAGE_TRACKS_NAME);
   
    currentSettings.tracks.clear();
    for (auto track : tracksPage->getTracks()) {
        TLTrackSetting ts;
        ts.name = track->getName();
        ts.type = track->getTrackType();
        currentSettings.tracks.push_back(ts);
    }
    
    currentSettings.markers.clear();
    for (auto marker : timelineView._markers){
        currentSettings.markers.push_back(marker);
    }
}


void TimelinePanel::loadTimelineTracksFromFolder(){
    string rootDir = SettingsManager::getInstance().getRootDir();
    timelineView.timeline.loadTracksFromFolder(rootDir + TIMELINE_SETTINGS_DIR"/");
}
void TimelinePanel::saveTimelineTracksToFolder(){
    string rootDir = SettingsManager::getInstance().getRootDir();
    timelineView.timeline.saveTracksToFolder(rootDir + TIMELINE_SETTINGS_DIR);
}









