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


#pragma mark - Core funcs

//-------------------------------------------------

void TimelinePanel::setup(int x, int y, int w, int h, string audiofile){
    
    BasePanel::setup(x, y, w, h);
    
    int gui_h = TL_GUI_COMP_HEIGHT;
    guiView.setup(x, y, w, gui_h, &timelineView);
    timelineView.setup(x, guiView.maxY(), w, h - guiView.getHeight(), audiofile);
    
}
//-------------------------------------------------
void TimelinePanel::update(){
    timelineView.update();
    guiView.update();
}
//-------------------------------------------------
void TimelinePanel::draw(){
    if (_isHidden){ return; }
    View::draw();
    TS_START("timeline");
    timelineView.draw();
    TS_STOP("timeline");
    
    TS_START("gui");
    guiView.draw();
    TS_STOP("gui");
    
}
//--------------------------------------------------------------
void TimelinePanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, TL_GUI_COMP_HEIGHT);
    timelineView.resize(x, guiView.maxY(), w, h - guiView.getHeight());
    
}
//--------------------------------------------------------------
void TimelinePanel::keyPressed(int key){
    timelineView.keyPressed(key);
}

//--------------------------------------------------------------
bool TimelinePanel::getFocused(){
    return guiView.gTrackName->getFocused();
}
void TimelinePanel::exit(){}
//--------------------------------------------------------------
#pragma mark - Settings
//--------------------------------------------------------------
void TimelinePanel::loadSettings(string rootDir){
    //TODO: Revisar esta funcion...
    
    //-:Load Xml file
    
    string filenamePanel = rootDir + TIMELINE_SETTINGS_DIR "/timePanel_settings.xml";
    ofxXmlSettings xml;
    
    if( xml.loadFile(filenamePanel) ){
        ofLogVerbose()<<"TimePanel: "<< filenamePanel <<" loaded.";
    }else{
        ofLogError()<< "TimePanel: unable to load " << filenamePanel ;
        return;
    }
    
    //----------------------------------
    //-:Add markers from loaded settings.
    
    timelineView.clearMarkers();
    
    int markersNum = xml.getValue("PANEL:MARKERS:MARKERS-NUM", 0);
    for (int i=0; i<markersNum; i++){
        float markerTimeMillis = xml.getValue("PANEL:MARKERS:MARKER-" + ofToString(i), 0.0);
        timelineView.addMarker(markerTimeMillis);
    }
    //----------------------------------
    //-:Create tracks from loaded settings.
    int tracksNum= xml.getValue("PANEL:TRACKS:TRACKS-NUM", 0);
    for (int i=0; i<tracksNum; i++){
        string trackName = xml.getValue("PANEL:TRACKS:TRACK-" + ofToString(i) +":NAME", "");
        string trackType = xml.getValue("PANEL:TRACKS:TRACK-" + ofToString(i) +":TYPE", "");
        
        auto tracksPage = timelineView.timeline.getPage(PAGE_TRACKS_NAME);
        
        //If track doesnt exist and its not default -> create it.
        if(trackName != "DEFAULT" &&
           tracksPage->getTrack(trackName)==NULL){
           
            if(trackType=="Curves"){
                timelineView.addTrack(trackName, CURVES);
            } else if(trackType=="Bangs"){
                timelineView.addTrack(trackName, BANGS);
            } else if(trackType=="Switches"){
                timelineView.addTrack(trackName, SWITCHES);
            } else if(trackType=="Notes"){
                timelineView.addTrack(trackName, NOTES);
            }
            
        }
    }
    //----------------------------------
    //-:Load Timeline Settings
    timelineView.timeline.loadTracksFromFolder(rootDir + TIMELINE_SETTINGS_DIR"/");
    timelineView.updateHeight();
    
    //----------------------------------
    //-:Log
    
    ofLogVerbose()<<"TimePanel: settings LOADED";
    ofLogVerbose()<<"-Tracks Loaded: "<< tracksNum;
    ofLogVerbose()<<"-Markers Loaded: "<< markersNum;
    

}
//--------------------------------------------------------------
void TimelinePanel::saveSettings(string rootDir){
    
    //-:Save Timeline Settings
    timelineView.timeline.saveTracksToFolder(rootDir + TIMELINE_SETTINGS_DIR);
    
    //-:Save Tracks Names and Types
    
    auto tracksPage = timelineView.timeline.getPage(PAGE_TRACKS_NAME);

    string filenamePanel = rootDir + TIMELINE_SETTINGS_DIR "/timePanel_settings.xml";
    ofxXmlSettings savedSettings;
    
    int tracksNum = tracksPage->getTracks().size();
    
    savedSettings.addTag("PANEL");
    savedSettings.pushTag("PANEL");
    savedSettings.addTag("TRACKS");
    savedSettings.pushTag("TRACKS");
    savedSettings.addValue("TRACKS-NUM", tracksNum);
    
    for (int i=0; i<tracksNum; i++){
        savedSettings.addTag("TRACK-"+ofToString(i));
        savedSettings.pushTag("TRACK-"+ofToString(i));
        savedSettings.addValue("NAME", tracksPage->getTracks()[i]->getName());
        savedSettings.addValue("TYPE", tracksPage->getTracks()[i]->getTrackType());
        savedSettings.popTag();
    }
    savedSettings.popTag();
    

    //-:Save Markers
    
    savedSettings.addTag("MARKERS");
    savedSettings.pushTag("MARKERS");
    
    savedSettings.addValue("MARKERS-NUM", (int)timelineView._markers.size());
    for (int i=0; i<timelineView._markers.size(); i++){
        savedSettings.addValue("MARKER-"+ofToString(i), timelineView._markers[i]);
    }
    
    savedSettings.saveFile(filenamePanel);
    
    //----------------------------------
    //-:Log
    
    ofLogVerbose()<<"TimePanel: settings SAVED";
    ofLogVerbose()<<"-Tracks Saved: "<< tracksNum;
    for (auto track : tracksPage->getTracks()){
        ofLogVerbose()<<"--Name: "<<track->getName() << "- Type: "<< track->getTrackType();
    }
    ofLogVerbose()<<"-Markers Saved: "<< timelineView._markers.size();
    
}







