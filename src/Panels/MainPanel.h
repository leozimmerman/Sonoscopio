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

#include "ofxDatGui.h"
#include "ofxXmlSettings.h"

#include "Macros.h"
#include "BasePanel.h"

#include "MainPanelGui.h"
#include "FileInfoView.h"
#include "Settings.h"

#define FILE_INFO_HEIGHT 20

class MainPanel : public BasePanel{
    
public:
    
    virtual void setup(int x, int y, int w, int h) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void exit() override;
    
    virtual bool getFocused() override;

    virtual void resize(int x, int y, int w, int h) override;
    
    void keyPressed(int key) override;
    
    void saveAllSettings();
    void renderAnalysis();
    
    void loadSettings(MainPanelSettings& settings);
    void updateCurrentSettings();

    void openFileDialog();
    
    void applySettings(int fps, int buffersize, float bpm, string host, int port);
    
    MainPanelSettings* getCurrentSettingsPtr(){
        return &currentSettings;
    }
    
    void setAudioFileInfo(string filename, string path,  float duration, int samplerate, int channels, int durationInFrames, int framerate){
        fileInfoView.setAudioFileInfo(filename, path, duration, samplerate, channels, durationInFrames, framerate);
    }
    
private:
    MainPanelGui guiView;
    FileInfoView fileInfoView;
    MainPanelSettings currentSettings;
    
    
    
};
