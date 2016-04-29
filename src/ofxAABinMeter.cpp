
#include "ofxAABinMeter.h"

//ofxAABinMeter::ofxAABinMeter() :  ofxAAMeter(int x, int y, int w, int h){
//    
//
//    
//}
//-------------------------------------------------------
void ofxAABinMeter::update(){

}
//-------------------------------------------------------
void ofxAABinMeter::draw(){
    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    
    
    //valueMeter-------------------------
    
    ofFill();
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    float bin_w = (float) _w / _nBins;
    for (int i = 0;i < _nBins; i++){
        float bin_h = -1 * (_values[i] * _h);
        ofDrawRectangle(i*bin_w, _h, bin_w, bin_h);
    }
    
    
//    float meter_h = -1 * (_h * _value);
//    ofDrawRectangle( _w * .25 , _h, _w * .5, meter_h);
    
    //label------------------------------
    //constraing width
    float label_w = verdana.stringWidth(_name);
    if(label_w >= _w){
        float space_ratio = 1 / (label_w / _w);
        verdana.setLetterSpacing(space_ratio);
    }
    //align center
    label_w = verdana.stringWidth(_name);
    int label_x =  _w * .5 - label_w *.5;
    //draw label
    float line_h = verdana.getLineHeight();
    verdana.drawString(_name, label_x , line_h * .75);
    
    ofPopMatrix();
    
    
    ofPopStyle();

}