
#include "ofxAABinMeter.h"

ofxAABinMeter::ofxAABinMeter(string name, int x, int y, int w, int h) :  ofxAAMeter(name, x,  y,  w,  h){
    
    _meterOrient = HORIZONTAL;
   
    onOffToggle->setWidth( _w*0.2, 0.15);
    onOffToggle->setHeight(line_h*0.75);
    onOffToggle->setPosition(_x + _w - _w*0.15, _y+2);
    
    smoothSlider->setWidth( _w*0.2, 0.0);
    smoothSlider->setHeight(line_h*0.75);
    smoothSlider->setPosition(_x + _w - _w*0.2, _y+2 + line_h*0.75);
    
}

//void ofxAABinMeter::update(){
//
//}
//-------------------------------------------------------
void ofxAABinMeter::draw(){
    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);

    //valueMeter
    drawMeter();
    drawLabel();
    
    onOffToggle->draw();
    smoothSlider->drawElemental();
    
    ofPopStyle();

}
//-------------------------------------------------------
void ofxAABinMeter::drawMeter(){
    
    ofFill();
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    float bin_w = (float) _w / _nBins;
    
    for (int i = 0;i < _values.size(); i++){
        float scaledValue = ofMap(_values[i], _minValue, _maxValue, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * _h);
        ofDrawRectangle(i*bin_w, _h, bin_w, bin_h);
    }
    
    ofPopMatrix();
    
    

}
//-------------------------------------------------------
void ofxAABinMeter::setPosAndSize(int x, int y, int w, int h){
    
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _drawRect.set(x, y, w, h);
    
    onOffToggle->setPosition(_x + _w - _w*0.2, _y+2);
    smoothSlider->setPosition(_x + _w - _w*0.2, _y+2 + line_h*0.75);

}