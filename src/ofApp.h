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

#include "ofxTimeMeasurements.h"

#include "MainPanel.h"
#include "TimelinePanel.h"
#include "MetersPanel.h"

#include "ofEvents.h"
#include "TextModal.h"
#include "ofxHotKeys.h"


#define MAIN_PANEL_HEIGHT 0.15
#define METER_PANEL_WIDTH 0.2

class ofApp : public ofBaseApp{
public:

    void setFrameRate(int fps);
    void onModalEvent(ofxModalEvent e);
    void errorSent(string & errorMessage);
    
    MainPanel mainPanel;
    TimelinePanel timelinePanel;
    MetersPanel metersPanel;
    
    static ofEvent<string> errorEvent;
    
private:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    
    void windowResized(int w, int h);
    void updatePanelsDimensions(int w, int h);
  
    void setupOsc();
    void setupModals();
    void setupOFContext();
    void setupTimeMeasurment();
    void setupPanels();
    void setupListeners();
    
    void showKeyboardShortcuts();
    void showErrorMessage(string message);
    
    void toggleTimeMeasurement();
    
    void setPanelsClicksEnabled(bool enabled);
    
    int _main_height;
    int _meters_width;
    
    shared_ptr<TextModal> mText;
    string queuedErrorMessage;
    
};


