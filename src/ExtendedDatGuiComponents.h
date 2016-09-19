//
//  ExtendedDatGuiComponents.hpp
//  Sonoscopio
//
//  Created by Leo on 9/15/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

//----------------------------------------------
class OnOffToggle : public ofxDatGuiToggle{
public:
    
    OnOffToggle(string label, bool enabled) : ofxDatGuiToggle(label, enabled){
        
        colorOn = ofColor::white;
        
        colorOff = ofColor::gray;
    
    };
    
    void setColors(ofColor on, ofColor off){
        colorOn = on;
        colorOff = off;
    }
    
    void drawTransparent(){
        if (mVisible) {
        
            ofPushStyle();
 
            ofNoFill();
            ofSetLineWidth(0.5);
            
            if (mEnabled == true){
                ofSetColor(colorOn);
                ofDrawRectangle(x, y, mStyle.width, mStyle.height);
                mFont->draw("ON", x+mLabel.x, y+mStyle.height/2 + mLabel.rect.height/2);
            }   else{
                ofSetColor(colorOff);
                ofDrawRectangle(x, y, mStyle.width, mStyle.height);
                mFont->draw("OFF", x+mLabel.x, y+mStyle.height/2 + mLabel.rect.height/2);
            }
            ofPopStyle();
        }
    }
    
private:
    ofColor colorOn;
    ofColor colorOff;

};
//----------------------------------------------
class CustomSlider : public ofxDatGuiSlider{
public:
    
    
    CustomSlider(string label, float min, float max, double val): ofxDatGuiSlider(label, min, max, val){
        
        colorFill   = ofColor::white;
        colorBack   = ofColor::darkGray;
        colorLabel  = ofColor::yellow;
        
    }
    
    
    ~CustomSlider(){
        setFocused(false);
    }
    
    void setColors(ofColor back, ofColor label, ofColor fill){
        colorBack = back;
        colorLabel = label;
        colorFill = fill;
    }
    

    
    void drawSimplified(){
        if (!mVisible) return;
        ofPushStyle();
        // slider bkgd //
        ofNoFill();
        ofSetLineWidth(0.5);
        ofSetColor(colorBack);
        ofDrawRectangle(x, y, mStyle.width, mStyle.height);
        // slider fill //
        if (mScale > 0){
            ofFill();
            ofSetColor(colorFill);
            ofDrawRectangle(x, y, mStyle.width * mScale, mStyle.height);
        }
        // numeric input field //
        
       // mInput->draw();
        
        //label
        ofSetColor(colorLabel);
        string displayStr = mLabel.text + ": " + ofToString(getValue());
        mFont->draw(displayStr , x+mLabel.x, y+mStyle.height/2 + mLabel.rect.height/2);
        //mFont->draw(ofToString(getValue()), x+mLabel.x, y+mStyle.height/2 + mLabel.rect.height/2);
        
        ofPopStyle();
        
    }
    
    void onMousePress(ofPoint m)
    {
        ofxDatGuiComponent::onMousePress(m);
//        if (mInput->hitTest(m)){
//            mInput->onFocus();
//        }   else if (mInput->hasFocus()){
//            mInput->onFocusLost();
//        }
    }
    
    void onMouseDrag(ofPoint m)
    {
        if (mFocused && mInput->hasFocus() == false){
//            float s = (m.x-x-mLabel.width)/mSliderWidth;//original
            float s = (m.x-x)/mStyle.width;
            if (s > .999) s = 1;
            if (s < .001) s = 0;
            // don't dispatch an event if scale hasn't changed //
            if (s == mScale) return;
            mScale = s;
            mValue = ((mMax-mMin) * mScale) + mMin;
            if (mTruncateValue) mValue = round(mValue, mPrecision);
            //setTextInput();
            dispatchSliderChangedEvent();
            
        }
    }
    
private:
    
    ofColor colorBack;
    ofColor colorFill;
    ofColor colorLabel;
   
    
};
//----------------------------------------------
class PeakMeterButton : public ofxDatGuiButton{

public:
    PeakMeterButton(string label) : ofxDatGuiButton(label){
        gralColor = ofColor::red;
    };
    
    void setColor(ofColor color){gralColor = color;}
    
    void draw(){
        
        if (mVisible) {
            // anything that extends ofxDatGuiButton has the same rollover effect //
            ofPushStyle();
            
            if (mStyle.border.visible) drawBorder();
            ofNoFill();
            ofSetColor(gralColor);
            mFont->draw(mLabel.text , x+mLabel.x, y+mStyle.height/2 + mLabel.rect.height/2);
            
            ofPopStyle();
        }
    
    }
    
private:
    ofColor gralColor;
    
    
};
