#pragma once

#include "ofMain.h"

class Panel{
    
    public:
    
        virtual void setup(int x, int y, int width, int height, ofBaseApp* appPtr) =0;
        virtual void update() =0;
        virtual void draw() =0;
        virtual void exit() =0;
        
        void setPosition(ofVec2f pos){_x = pos.x; _y = pos.y;}
        void setWidth(int w){_w = w ;}
        void setHeight(int h){_h = h;}
        void setBackgroundColor(ofColor col){_bckgColor = col;}
        
        ofVec2f getPosition(){return ofVec2f(_x, _y);}
        int getWidth(){return _w;}
        int getHeight(){return _h;}

        ofColor getBackgroundColor(){return _bckgColor;}
    
    
    
    protected:
    
        int _x, _y;
        int _w, _h;
        ofColor _bckgColor;
    
};
