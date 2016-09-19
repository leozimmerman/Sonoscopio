
#include "ofxAABinMeter.h"

ofxAABinMeter::ofxAABinMeter(string name, int x, int y, int w, int h) :  ofxAAMeter(name, x,  y,  w,  h){
    
    _meterOrient = HORIZONTAL;
    
    smoothSlider->setHeight(_line_h*0.75);
    onOffToggle->setHeight(_line_h*0.75);
    
    updateComponentsPositions();
    updateComponentsWidth();
}

//-------------------------------------------------------
void ofxAABinMeter::draw(){

    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    ofPopStyle();
    
    drawLabel();
    
    if(_enabled) drawMeter();
    
    if(_bDrawFullDisplay){
        //spectrum cant be turned off
         //mfcc cant work if melBands is turn off
        if(getName() != MTR_NAME_SPECTRUM &&
           getName() != MTR_NAME_MFCC){
            onOffToggle->drawTransparent();
        }
        
        if(_enabled) smoothSlider->drawSimplified();
    }
    

}
//-------------------------------------------------------
void ofxAABinMeter::drawMeter(){
    
    if(getEnabled()==false) return;
    //-----------------------------
    
    ofFill();
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofPushStyle();
    ofSetColor(COLOR_RECT_METER, COLOR_RECT_METER_ALPHA);
    
    
    float bin_w = (float) _w / _nBins;
    
    for (int i = 0;i < _values.size(); i++){
        float scaledValue = ofMap(_values[i], _minEstimatedValue, _maxEstimatedValue, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * _h);
        ofDrawRectangle(i*bin_w, _h, bin_w, bin_h);
    }
    
    ofPopStyle();
    
    ofPopMatrix();


}
//-------------------------------------------------------
void ofxAABinMeter::drawLabel(){
    
    ofPushMatrix();
    
    ofTranslate(_x, _y);
    
    if(_enabled) ofSetColor(_mainColor);
    else ofSetColor(COLOR_ONOFF_OFF);
    
    verdana->drawString(_name, _label_x, _line_h * 0.75);
    
    ofPopMatrix();
}


//-------------------------------------------------------
void ofxAABinMeter::setPosAndSize(int x, int y, int w, int h){
    
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _drawRect.set(x, y, w, h);
    
    updateComponentsPositions();
    updateComponentsWidth();
    //updateColors();
    
}
//-------------------------------------------------------
void ofxAABinMeter::updateComponentsPositions(){
   
    smoothSlider->setPosition(_x + _w - _w * 0.3, _y);
    
    if(_name != MTR_NAME_SPECTRUM &&
       _name != MTR_NAME_MFCC){
        
        onOffToggle->setPosition(_x, _y);
        
    }else{
        onOffToggle->setPosition(_x + _w *10 , _y + _h *10);//-->OUT OF SCREEN! Spectrum and MFCC, cant be turned off
    }

}
//-------------------------------------------------------
void ofxAABinMeter::updateComponentsWidth(){
    
    smoothSlider->setWidth( _w * 0.3, 0.0);
    onOffToggle->setWidth( _w*0.25, 0.0);

    

}
