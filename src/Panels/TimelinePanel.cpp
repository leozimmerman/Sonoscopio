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
#include "ofApp.h"
#include "SettingsManager.h"
#include "FileManager.h"
#include "AnalysisDataSaver.h"
#include "OscSender.h"
#include "PanelsBridge.h"

#pragma mark - Core funcs

void TimelinePanel::setup(int x, int y, int w, int h){
    BasePanel::setup(x, y, w, h);
    BasePanel::setEnabled(false);
    setupSingletons();
    
    guiView.setup(x, y, w, GUI_COMP_HEIGHT * 3, &timelineView);
    timelineView.setup(x, guiView.maxY(), w, h - guiView.getHeight());
}

void TimelinePanel::setupSingletons(){
    SettingsManager::getInstance().setTimelinePanelPtr(this);
    FileManager::getInstance().setTimelinePanelPtr(this);
    AnalysisDataSaver::getInstance().setTimelinePanelPtr(this);
    OscSender::getInstance().setTimelinePanelPtr(this);
    PanelsBridge::getInstance().setTimelinePanelPtr(this);
}

void TimelinePanel::update(){
    if(!enabled){return;}
    
    timelineView.update();
    guiView.update();
}

void TimelinePanel::draw(){
    if(!enabled){return;}
    
    if (!View::mustDrawNewFrame()){
        View::drawLoadedTexture();
        return;
    }
    
    if (_isHidden){ return; }
    View::draw();
    
    TS_START("gui-tl");
    guiView.draw();
    TS_STOP("gui-tl");
    
    TS_START("timeline");
    timelineView.draw();
    TS_STOP("timeline");
    
    View::loadViewInTexture();
    
}

void TimelinePanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w,  GUI_COMP_HEIGHT * 3);
    timelineView.resize(x, guiView.maxY(), w, h - guiView.getHeight());
}

bool TimelinePanel::keyPressed(int key){
    return timelineView.keyPressed(key);
}

bool TimelinePanel::getFocused(){
    return guiView.getFocused();
}
void TimelinePanel::exit(){}


#pragma mark - Settings

void TimelinePanel::loadSettings(TimelinePanelSettings& settings){
    currentSettings = settings;
    guiView.setStateFromSettings(currentSettings);
    timelineView.setStateFromSettings(currentSettings);
}

void TimelinePanel::updateCurrentSettings(){
    guiView.loadStateIntoSettings(&currentSettings);
    timelineView.loadStateIntoSettings(&currentSettings);
}










