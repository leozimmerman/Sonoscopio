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
#include "ofxDatGui.h"
#include "ExtendedDatGuiComponents.h"
#include "Macros.h"
#include "View.h"

class OnOffEventData{
public:
    string name;
    bool state;
    int panelId;
};



//-:Meters Colors
//#define COLOR_MAIN_A ofColor::darkSeaGreen
//#define COLOR_MAIN_B ofColor::darkKhaki


#define COLOR_MAIN_A ofColor::paleTurquoise
#define COLOR_MAIN_B ofColor::paleGoldenRod

#define COLOR_PEAKS ofColor::crimson
#define COLOR_SMTH_LABEL ofColor::mediumOrchid

//#define COLOR_ONOFF_ON ofColor::chartreuse
#define COLOR_ONOFF_OFF ofColor::dimGray

#define COLOR_RECT_METER ofColor::white
#define COLOR_RECT_METER_ALPHA 40


class MeterView : public View {
public:
    
    MeterView(string name, int x, int y, int w, int h, int panelId);
    virtual ~MeterView();


    //void setup();
    virtual void update();
    virtual void draw();

    virtual void drawLabel();
    virtual void drawMeter();
    virtual void drawValueDisplay();
    
    void resetPeak();
    virtual void resize(int x, int y, int w, int h);
    
    string getName(){return _name;}
    ofColor getMainColor(){return _mainColor;}
    ofColor getBackgroundColor(){return _backgroundColor;}
    float getValue(){return _value;}
    int getPositionX(){return _x;}
    int getPositionY(){return _y;}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    float getMinEstimatedValue(){return _minEstimatedValue;}
    float getMaxEstimatedValue(){return _maxEstimatedValue;}
    
    float getSmoothAmnt(){return _smoothAmnt;}
    bool getEnabled(){return _enabled;}
    bool getIsFullDisplay(){return _bDrawFullDisplay;}

    void setName(string name){_name = name;}
    void setMainColor(ofColor col);
    void setBackgroundColor(ofColor col){_backgroundColor = col;}
    void setValue(float val);
    void setNormalizedValue(float val);
    
    virtual void updateComponentsPositions();
    virtual void setYandHeight(int y, int h);
    virtual void updateComponentsWidth();
    void setHeight(float h);
    void setMinEstimatedValue(float val){_minEstimatedValue = val;}
    void setMaxEstimatedValue(float val){_maxEstimatedValue = val;}
    void setSmoothAmnt(float val);
    void setEnabled(bool state);
    void setFullDisplay(bool b){_bDrawFullDisplay = b;}

    ofRectangle getDrawRect(){return _drawRect;}
    
    virtual void onSliderEvent(ofxDatGuiSliderEvent e);
    virtual void onButtonEvent(ofxDatGuiButtonEvent e);
    
    static ofEvent<OnOffEventData> onOffEventGlobal;//this is a shared event for all the instances of this class, so any instance of this class will broadcast to the same event,
    
    CustomSlider * smoothSlider;
    OnOffToggle* onOffToggle;
    PeakMeterButton* peakButton;
    
    
protected:
    
    float _minEstimatedValue, _maxEstimatedValue;
    
    int _x, _y;
    int _w, _h;
    
    int _panelId; //for OnOff Event Listeners
    
    ofRectangle _drawRect;
    
    ofColor _mainColor;
    ofColor _backgroundColor;
    
    ofTrueTypeFont*	verdana;
    
    bool _bDrawFullDisplay;
    
    string  _name;
    bool    _enabled;
    
    float   _line_h;
    int     _label_x;
    
private:
    
    float _value;
    float _valueNorm;
    float _maxValueRegistered;//peak
    float _smoothAmnt;

    

};
