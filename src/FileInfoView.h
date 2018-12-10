//
//  FileInfoView.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 06/12/2018.
//

#pragma once

#include "ofMain.h"
#include "View.h"

class FileInfoView : public View {
    
public:
    void setup(int x, int y, int w, int h) override;
    //void resize(int x, int y, int w, int h) override;
    void draw() override;
    
    void setAudioFileInfo(string fileName, string path,  float duration, int samplerate, int channels, int durationInFrames, int framerate);
    
private:
    std::string infoStr;
    ofTrueTypeFont    verdana;
};
