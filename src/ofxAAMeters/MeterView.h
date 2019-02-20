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
//#include "Macros.h"
#include "View.h"
#include "ofxAudioAnalyzerUnit.h"
#include "MeterModal.h"

#define SMOOTHING_LABEL "SMTH"
#define ON_LABEL "ON"
#define PEAK_LABEL "PEAK"
#define CONFIG_LABEL "CONFIG"

#define COLOR_MAIN_A ofColor::paleTurquoise
#define COLOR_MAIN_B ofColor::paleGoldenRod

#define COLOR_PEAKS ofColor::crimson
#define COLOR_SMTH_LABEL ofColor::mediumOrchid

//#define COLOR_ONOFF_ON ofColor::chartreuse
#define COLOR_ONOFF_OFF ofColor::dimGray

#define COLOR_RECT_METER ofColor::white
#define COLOR_RECT_METER_ALPHA 50


class MeterView : public View {
public:
#pragma mark  - Static
    static MeterView* createMeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr);
    static int height;
    
#pragma mark - Instanced
    
    MeterView(ofxAAAlgorithmType algorithmType, int panelId,  ofxAudioAnalyzerUnit * aaPtr);
    virtual ~MeterView();

    virtual void update();
    void draw() override;

    virtual void resize(int x, int y, int w, int h) override;
    
    
    
    string getName(){return _name;}
    ofColor getMainColor(){return _mainColor;}

    float getValue(){return _value;}
    int getPositionX(){return _x;}
    int getPositionY(){return _y;}
    int getWidth(){return _w;}
    int getHeight(){return _h;}
    float getMinEstimatedValue(){return _minEstimatedValue;}
    float getMaxEstimatedValue(){return _maxEstimatedValue;}
    
    float getSmoothAmnt(){return _smoothAmnt;}
    bool getEnabled(){return _enabled;}
    bool getPlotterEnabled(){return _plotterEnabled;}
    ofxAAAlgorithmType getType(){return _algorithmType;}
    
    void toggleValuePlotter(bool enabled);
    void setMainColor(ofColor col);
    void setValue(float value);
    void setNormalizedValue(float value);
    void setMinEstimatedValue(float value);
    void setMaxEstimatedValue(float value);
    void setSmoothAmnt(float val);
    void setEnabled(bool state);
    
protected:
    void initDefaultValues();
    void initComponents();
    
    virtual void updateComponents();
    virtual void updateValues();
    virtual void updatePeak();
    
    void drawBounds();
    virtual void drawLabel();
    virtual void drawMeter();
    virtual void drawValueDisplay();
    
    virtual void renderDraw() override;
    virtual void drawStaticElements();
    virtual void drawValueElements();
    
    void resetPeak();
    
    virtual void setComponentsPositions();
    virtual void setComponentsWidth();
    void adjustFontLetterSpacing(int width);
    
    virtual void onSliderEvent(ofxDatGuiSliderEvent e);
    virtual void onButtonEvent(ofxDatGuiButtonEvent e);
    
    CustomSlider * smoothSlider;
    OnOffToggle* onOffToggle;
    TransparentMeterButton* peakButton;
    TransparentMeterButton* configButton;
    
    ofxAudioAnalyzerUnit* _audioAnalyzer;
    ofxAAAlgorithmType _algorithmType;
    float _minEstimatedValue, _maxEstimatedValue;
    int _panelId; //for OnOff Event Listeners
    ofColor _mainColor;
    ofTrueTypeFont*	font;
    
    float   _smoothAmnt;
    string  _name;
    bool    _enabled;
    bool    _plotterEnabled;
    float   _line_h;
    int     _label_x;
    
private:
    void setupMenu();
    void showMenu();
    
    ofxDatGuiValuePlotter* valuePlotter;
    
    float _value;
    float _valueNorm;
    float _maxValueRegistered;//peak
    
    shared_ptr<MeterModal> menuModal;
};
