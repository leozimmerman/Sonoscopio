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
#include "Panel.h"

class MainPanel : public Panel{

    public:
    
        void setup(int x, int y, int width, int height, ofBaseApp* appPtr);
        void update();
        void draw();
        void exit();
    
        void drawFileInfo();
    
        void setFileInfoString(string str){fileInfoStr = str;}
        void openOpenFileDialog();
        void processOpenFileSelection(ofFileDialogResult openFileResult);
    
        void resize(int w, int h);
    
        void setupGui();
        void adjustGui(int w, int h);
    
        vector<ofxDatGuiComponent*> components;
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onProjectDropdownEvent(ofxDatGuiDropdownEvent e);
        void onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e);
    
        void loadSettings(string rootDir="");
        void saveSettings(string rootDir="");
    
        bool getFocused();
    
    private:
    
        string fileInfoStr;
        string fileName;
        ofTrueTypeFont	verdana;
        int  _guiCompWidth;
    
        ofColor bordCol;
        int bordWidth;
    
        ofColor fileinfoFontCol;
    
        ofxDatGuiSlider* gVolume;
        ofxDatGuiToggle* gSplit;
        ofxDatGuiToggle* gLoop;
        ofxDatGuiToggle* gSendOsc;
        ofxDatGuiToggle* gShowBpm;
        ofxDatGuiToggle* gSnapBpm;
        ofxDatGuiToggle* gFramebased;
        ofxDatGuiFRM* gFpsMonitor;
        //REMOVE
        ofxDatGuiTextInput* gHost;
        ofxDatGuiTextInput* gPort;
        ofxDatGuiTextInput* gBpm;
        ofxDatGuiTextInput* gFps;
        ofxDatGuiDropdown* gBufferSize;
    
        string _panelDir;
    
        ofDirectory projects_dir;
    

};
