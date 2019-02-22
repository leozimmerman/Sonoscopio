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
    void setup(int x, int y, int w, int h) override;
    void update() override;
    void draw() override;
    void exit() override;
    
    void resize(int x, int y, int w, int h) override;
    
    bool getFocused() override;
    bool keyPressed(int key) override;
    
    void setClicksEnabled(bool enabled) override;
    
    void saveAllSettings();
    void renderAnalysis();
    
    void resetSettings();
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
    void setupSingletons() override;
    
    MainPanelGui guiView;
    FileInfoView fileInfoView;
    MainPanelSettings currentSettings;
    
};
