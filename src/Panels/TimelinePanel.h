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
#include "ofxDatGui.h"

#include "Macros.h"
#include "Panel.h"

#define PAGE_AUDIO_NAME "PageAudio"
#define PAGE_TRACKS_NAME "PageTracks"

#define TL_GUI_COMP_HEIGHT 26

enum trackType {
    CURVES,
    BANGS,
    SWITCHES,
    NOTES
};


class TimelinePanel : public Panel{
    
public:
    
    void setup(int x, int y, int width, int height, ofBaseApp* appPtr);
    void update();
    void draw();
    void exit(){};
    
    void keyPressed(int key);
    
    void setFrameRate(int fps);
    
    void loadSettings(string rootDir="");
    void saveSettings(string rootDir="");
    
    void setupGui();
    void adjustGuiSize(int y, int w, int h);
    
    void resize(int y, int w, int h);
    void resizeHeight(int tl_h);
    
    void addMarker();
    void addMarker(float millis);
    void clearMarkers();
    
    void openAudioFile(string filename);
    string getFileInfo();
    
    
    
    bool getIfIsDragging(){return _footerIsDragging;}
    bool getIsHidden(){return _isHidden;}
    
    void checkIfHeightChanged();
    void checkIfWaveformPreviewChanged();
    
    void addTrack(string name, trackType type);
    void removeTrack(string name);
    void toggleShowTracks();
    void adjustTracksHeight();
    void toggleEnableDisableFocusedTrack();
    void expandFocusedTrack();
    
    void addKeyframeInFocusedTrack();
    
    void hideTracks();
    
    std::map<string, float> getTracksValues();

    void bangFired(ofxTLBangEventArgs& args);
    
    ofxTimeline timeline;
    ofxTLAudioTrack* audioTrack;
    
    ofEvent<int> heightResizedEvent;
    
    bool getFocused();
    void setHidden(bool h);
    
private:
    
    int _frameRate;
    
    int _guiCompHeight;
    string currentTrackName;
    trackType currentTrackType;
    
    bool _isThereBang = false;
    ofxTLTrack* _bangedTrack;
    
    ofColor waveformCol;
    
    ofColor bordCol;
    int bordWidth;
    
    bool _footerIsDragging;
    
    vector<float> _markers;
    
    vector<ofxDatGuiComponent*> components;
    ofxDatGuiTextInput* gTrackName;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    bool _isHidden = false;
    
    //margins related to ofxTimeline height auto adjust func.
    float _strangeMargin1 = 66.0;
    float _strangeMargin2 = 36.5;
    
};

