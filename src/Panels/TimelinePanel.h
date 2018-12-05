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
#include "TimelineViewGui.h"
#include "TimelineView.h"
#include "Settings.h"

class TimelinePanel : public BasePanel {
    
public:
    
    void setup(int x, int y, int w, int h, string audiofile);
    virtual void update() override;
    virtual void draw() override;
    virtual void exit() override;
    
    void keyPressed(int key) override;
    virtual bool getFocused() override;
    
    virtual void resize(int x, int y, int w, int h) override;

    void loadSettings();
    void updateCurrentSettings();
    
    void saveTimelineTracksToFolder();
    void loadTimelineTracksFromFolder();

    void stop(){ timelineView.timeline.stop(); }
    
    void openAudioFile(string filename){
        timelineView.openAudioFile(filename);
    }
    
    void addKeyframeInFocusedTrack(){
        timelineView.addKeyframeInFocusedTrack();
    }
    
    TimelinePanelSettings* getCurrentSettingsPtr(){
        return &currentSettings;
    }
    
    #pragma mark - Setters
    //TODO: Remove this.
    void setCurrentFrame(int currentFrame){
        timelineView.timeline.setCurrentFrame(currentFrame);
    }
    
    #pragma mark - Getters
    bool isFileLoaded(){return timelineView.isSoundLoaded();}
    bool isPlaying(){return timelineView.timeline.getIsPlaying();}
    string getFileInfo(){return timelineView.getFileInfo();};
    int getSampleRate(){return timelineView.audioTrack->getSampleRate();}
    int getNumChannels(){return timelineView.audioTrack->getNumChannels();}
    
    std::map<string, float> getTracksValues(){
        return timelineView.getTracksValues();
    }
    ofSoundBuffer& getSoundBufferForFrame(int frame, int size) {
        return timelineView.audioTrack->getSoundBufferForFrame(frame, size);
    }
    ofSoundBuffer& getSoundBufferMonoForFrame(int frame, int size){
        return timelineView.audioTrack->getSoundBufferMonoForFrame(frame, size);
    }
    ofSoundBuffer& getCurrentSoundBuffer(int size){
        return timelineView.audioTrack->getCurrentSoundBuffer( size);
    }
    ofSoundBuffer& getCurrentSoundBufferMono(int size){
        return timelineView.audioTrack->getCurrentSoundBufferMono(size);
    }
    int getTotalFramesNum(){return timelineView.timeline.getDurationInFrames();}
    string getSoundfilePath(){return timelineView.audioTrack->getSoundfilePath();}
    float getDurationInSeconds(){return timelineView.timeline.getDurationInSeconds();}
    float getBpm(){return timelineView.timeline.getBPM();}
    
private:
    TimelineViewGui guiView;
    TimelineView timelineView;
    TimelinePanelSettings currentSettings;
    
    
};

