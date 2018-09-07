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
#include "View.h"
#include "ofxDatGui.h"

class BasePanel : public View {
    
public:
    
    virtual void setup(int x, int y, int w, int h);
    virtual void update() =0;
    virtual void exit() =0;
    
    virtual bool getFocused() =0; //???
    virtual void resize(int x, int y, int w, int h) =0;
    virtual void saveSettings(string rootDir="") =0;
    virtual void loadSettings(string rootDir="") =0;
    
    bool getIsHidden(){return _isHidden;}
    virtual void setHidden(bool h) {_isHidden = h;}
    
protected:
    //TODO: Remove this from here!
    vector<ofxDatGuiComponent*> components;
    bool _isHidden = false;
    ofColor bordCol;
    int bordWidth;
    int _guiCompHeight;
    int  _guiCompWidth;
    //--------------------
    string _panelDir;
    
};
