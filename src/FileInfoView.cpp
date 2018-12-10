//
//  FileInfoView.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 06/12/2018.
//

#include "FileInfoView.h"

void FileInfoView::setup(int x, int y, int w, int h){
    View::setup(x, y, w, h);
    setBackgroundColor(40);
    
    ofTrueTypeFont::setGlobalDpi(72);
    verdana.load("gui_assets/fonts/verdana.ttf", 12, true, true);
    verdana.setLineHeight(13.0f);
    verdana.setLetterSpacing(1.037);
    infoStr = "non file loaded";
}

void FileInfoView::draw(){
    View::draw();
    ofPushStyle();
    ofSetColor(ofColor::darkKhaki);
    verdana.drawString(infoStr,  10, _y+13);
    ofPopStyle();
}

void FileInfoView::setAudioFileInfo(string fileName, string path,  float duration, int samplerate, int channels, int durationInFrames, int framerate){
    
    infoStr = "File name: " + fileName + "-duration: " + ofToString(duration) + "sec."
    "-sample rate: " + ofToString(samplerate) +
    "-channels: " + ofToString(channels) +
    "-frames: " + ofToString(durationInFrames) + " at " + ofToString(framerate) + "fps" + " | path: " + path;
}
