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


#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"


#include "ofxDatGui.h"

#include "Macros.h"
#include "BasePanel.h"
#include "TLGuiView.h"
#include "TimelineView.h"



#define TL_GUI_COMP_HEIGHT 26 //TODO: Borrar



class TimelinePanel : public BasePanel {
    
public:
    
    void setup(int x, int y, int w, int h, string audiofile);
    virtual void update() override;
    virtual void draw() override;
    virtual void exit() override;
    
    virtual bool getFocused() override;
    
    virtual void resize(int x, int y, int w, int h) override;
    virtual void saveSettings(string rootDir="") override;
    virtual void loadSettings(string rootDir="") override;
    
    
    void keyPressed(int key);
    
    
    TimelineView timelineView;//TODO: Make it private
    
private:
    TLGuiView guiView;
    
    
};

