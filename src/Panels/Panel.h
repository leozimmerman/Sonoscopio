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

class Panel{
    
    public:
    
        virtual void setup(int x, int y, int width, int height, ofBaseApp* appPtr){};
        virtual void update() =0;
        virtual void draw() =0;
        virtual void exit() =0;
    
        void drawBackground(){
            ofPushStyle();
            ofSetColor(getBackgroundColor());
            ofDrawRectangle(_x, _y, _w, _h);
            ofPopStyle();
        }
    
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
