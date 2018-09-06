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
#include "BasePanel.h"

#define PAGE_AUDIO_NAME "PageAudio"
#define PAGE_TRACKS_NAME "PageTracks"

#define TL_GUI_COMP_HEIGHT 26

enum trackType {
    CURVES,
    BANGS,
    SWITCHES,
    NOTES
};


class TimelinePanel : public BasePanel {
    
public:
    
    void setup(int x, int y, int width, int height, string audiofile);
    virtual void update() override;
    virtual void draw() override;
    virtual void exit() override;
    
    virtual bool getFocused() override;
    virtual void setupGui() override;
    virtual void resize(int x, int y, int w, int h) override;
    virtual void saveSettings(string rootDir="") override;
    virtual void loadSettings(string rootDir="") override;
    
    void setupTimeline(string audiofile);
    void keyPressed(int key);
    
    void setFrameRate(int fps);
    
    void adjustGuiSize(int y, int w, int h);
    
    void addMarker();
    void addMarker(float millis);
    void clearMarkers();
    
    void openAudioFile(string filename);
    string getFileInfo();
    
    bool getIfIsDragging(){return _footerIsDragging;}
    
    //void checkIfHeightChanged();
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
    
    
private:
    
    int _frameRate;
    
    string currentTrackName;
    trackType currentTrackType;
    
    bool _isThereBang = false;
    ofxTLTrack* _bangedTrack;
    ofColor waveformCol;
    bool _footerIsDragging;
    
    vector<float> _markers;
    
    ofxDatGuiTextInput* gTrackName;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    //margins related to ofxTimeline height auto adjust func.
    float _strangeMargin1 = 66.0;
    float _strangeMargin2 = 36.5;
    
};

