
#include "ofxAABinMeter.h"

//ofxAABinMeter::ofxAABinMeter() :  ofxAAMeter(int x, int y, int w, int h){
//    
//
//    
//}

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
    
    onOffToggle->draw();

}