
#pragma once

#include "ofMain.h"
#include "Panel.h"
#include "ofxDatGui.h"



class MainPanel : public Panel{

    public:
    
        void setup(int x, int y, int width, int height, ofBaseApp* appPtr);
        void update();
        void draw();
        void exit();
    
        vector<ofxDatGuiComponent*> components;
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
    
        void setFileInfoString(string str){fileInfoStr = str;}
    
        void openOpenFileDialog();
        void processOpenFileSelection(ofFileDialogResult openFileResult);
    
    
  
    
    private:
        string fileInfoStr;
        string fileName;
        ofTrueTypeFont	verdana14;
        int guiCompHeight, guiCompWidth;

};
