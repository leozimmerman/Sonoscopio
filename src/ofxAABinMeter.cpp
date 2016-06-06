
#include "ofxAABinMeter.h"

ofxAABinMeter::ofxAABinMeter(string name, int x, int y, int w, int h) :  ofxAAMeter(name, x,  y,  w,  h){
    
    _meterOrient = HORIZONTAL;
    
    smoothSlider->setWidth( _w*0.2, 0.0);
    smoothSlider->setHeight(line_h*0.75);
    smoothSlider->setPosition(_x + _w - _w*0.2, _y+2);
    
    onOffToggle->setWidth( _w*0.125, 0.3);
    onOffToggle->setHeight(line_h*0.75);
    onOffToggle->setPosition(_x + _w - _w*0.15, _y+2 + line_h);
    
}

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
    
    if(_bDrawFullDisplay){
        //spectrum cant be turned off
         //mfcc cant work if melBands is turn off
        if(getName() != MTR_NAME_SPECTRUM &&
           getName() != MTR_NAME_MFCC){
            onOffToggle->draw();
        }
        
        smoothSlider->drawElemental();
    }
    

}
//-------------------------------------------------------
void ofxAABinMeter::drawMeter(){
    
    if(getEnabled()==false) return;
    //-----------------------------
    
    ofFill();
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    float bin_w = (float) _w / _nBins;
    
    for (int i = 0;i < _values.size(); i++){
        float scaledValue = ofMap(_values[i], _minEstimatedValue, _maxEstimatedValue, 0.0, 1.0, true);//clamped value
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
    
    
    smoothSlider->setPosition(_x + _w - _w*0.2, _y+2);
    
    if(getName() != MTR_NAME_SPECTRUM &&
       getName() != MTR_NAME_MFCC){
    
        onOffToggle->setPosition(_x + _w - _w*0.2, _y+2 + line_h*0.75);
    
    }else{
        onOffToggle->setPosition(_x + _w *10 , _y + _h *10);//out of screen
    }
    
    

}