//
//  View.hpp
//  Sonoscopio
//
//  Created by Leo on 04/09/2018.
//

#pragma once
#include "ofMain.h"

class View {

public:
    virtual void setup(int x, int y, int w, int h);
    virtual void resize(int x, int y, int w, int h);
    virtual void draw();
    
    int maxX() {return _x + _w;}
    int maxY() {return _y + _h;}
    
    void setPosition(ofVec2f pos){_x = pos.x; _y = pos.y;}
    void setBackgroundColor(ofColor col){backgroundColor = col;}
    
    ofVec2f getPosition(){return ofVec2f(_x, _y);}
    ofVec2f getSize(){return ofVec2f(_w, _h);}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    ofColor getBackgroundColor(){return backgroundColor;}
    
    bool getIsHidden(){return _isHidden;}
    virtual void setHidden(bool h) {_isHidden = h;}
    
    std::shared_ptr<View> parent;
    std::vector<std::shared_ptr<View>> subviews;
    
    
protected:
    int _x, _y;
    int _w, _h;
    ofColor backgroundColor;
    bool _isHidden = false;
};
