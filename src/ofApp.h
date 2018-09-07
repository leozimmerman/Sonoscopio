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

#pragma once

#include "ofMain.h"

#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"

#include "ofxTimeMeasurements.h"
#include "ofxAudioAnalyzer.h"

#include "MainPanel.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"

#include "AnalysisDataSaver.h"
#include "ofEvents.h"

#include "ofxModal.h"
#include "CustomModals.h"

#include "ofxHotKeys.h"
#include "Configurations.h"
#include "OscSender.h"

#define MAIN_PANEL_HEIGHT 0.15
#define METER_PANEL_WIDTH 0.2


class ofApp : public ofBaseApp{

  public:
 
    
    void setup();
	void update();
	void draw();
    void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void setupOFContext();
    void setupTimeMeasurment();
    void setupPanels();
    void setupListeners();
    void setupConfiguration();
    void setupOsc();
    void setupModals();
    
    void togglePlay();
    void stop();
    void rewind();
    void addKeyframeInFocusedTrack();
    
    void openAudioFile(string filename);
    void onTimelinePanelResize(int & h);
    
    //void hideMetersPanel(bool hide);
    void setFrameRate(int fps);
    void setAnalysisMode(AnalysisMode mode);
    //void setViewMode(ViewMode mode);
    void resetAnalysisEngine();
    void setBufferSize(int bs);
    void updatePanelsDimensions(int w, int h);
    
    //TODO: Fix this...
    int getTotalFramesNum(){return timePanel.timelineView.timeline.getDurationInFrames();}
    string getSoundfilePath(){return timePanel.timelineView.audioTrack->getSoundfilePath();}
    float getDurationInSeconds(){return timePanel.timelineView.timeline.getDurationInSeconds();}
    float getBpm(){return timePanel.timelineView.timeline.getBPM();}
    
    void saveSettings();
    void loadSettings();
    
    void openProject(string projectDir);
    
    void saveAnalysisDataToFile();
    void drawSavingAnalysisSign();
    
    void onsetFired(int & panelId);
    
    shared_ptr<TextModal> mText;
    shared_ptr<MenuModal> mMenu;
    
    void onModalEvent(ofxModalEvent e);
    void showKeyboardShortcuts();
    void showMenu();

    MainPanel mainPanel;
    TimelinePanel timePanel;
    MetersPanel metersPanel;
    
    ofxAudioAnalyzer mainAnalyzer;
    AnalysisDataSaver dataSaver;
    MainConfiguration config;
    OscSender oscSender;
    
private:
    
    ofSoundBuffer soundBuffer;
    ofMutex audioMutex;
    ofTrueTypeFont	verdana; //Esto??
    
    int _main_height;
    int _meters_width;
    
    
    
};


