//
//  View.cpp
//  Sonoscopio
//
//  Created by Leo on 04/09/2018.
//

#include "View.h"

void View::setup(int x, int y, int w, int h){
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    _framerate = ofGetTargetFrameRate();
    backgroundColor = ofColor::white;
    texView.allocate(w, h, GL_RGB);
}

void View::draw(){
    ofPushStyle();
    ofSetColor(getBackgroundColor());
    ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
}

void View::resize(int x, int y, int w, int h) {
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    texView.allocate(w, h, GL_RGB);
    renderDraw();
}


void View::drawLoadedTexture() {
    ofSetHexColor(0xffffff);
    texView.draw(_x, _y, _w, _h);
}

void View::loadViewInTexture(){
     texView.loadScreenData(_x, _y, _w, _h);
}

bool View::mustDrawNewFrame() {
    float target = ofGetTargetFrameRate();
    if (_framerate >= target) {return true;}
    
    int divider = (int) target / _framerate;
    if (ofGetFrameNum() % divider == 0) {return true;}
    
    return false;
}




