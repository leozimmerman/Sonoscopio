
#pragma once

#include "ofMain.h"
#include "Panel.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"

#define MAIN_SETTINGS_DIR "main_settings"

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
    
        void setupGUI();
        vector<ofxDatGuiComponent*> components;
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
        void loadSettings(string rootDir="");
        void saveSettings(string rootDir="");
    
    
    private:
        string fileInfoStr;
        string fileName;
        ofTrueTypeFont	verdana;
        int  guiCompWidth, guiCompHeight;
    
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
    
        string _panelDir;
    
        ofDirectory projects_dir;
    

};
