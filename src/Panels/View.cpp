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
    backgroundColor = ofColor::white;
}

void View::draw(){
    ofPushStyle();
    ofSetColor(getBackgroundColor());
    ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
//    for (auto v : subviews) {
//        v->draw();
//    }
}

void View::resize(int x, int y, int w, int h) {
    _x = x;
    _y = y;
    _w = w;
    _h = h;
}
