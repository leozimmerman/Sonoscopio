
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
        ofxDatGuiTextInput* gHost;
        ofxDatGuiTextInput* gPort;
        ofxDatGuiTextInput* gBpm;
        ofxDatGuiTextInput* gFps;
        ofxDatGuiFRM* gFpsMonitor;
        ofxDatGuiDropdown* gBufferSize;
    
        string _panelDir;
    
        ofDirectory projects_dir;
    

};
