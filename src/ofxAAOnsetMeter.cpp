
#include "ofxAAOnsetMeter.h"

//------------------------------------
ofxAAOnsetMeter::ofxAAOnsetMeter(int x, int y, int w, int h) : ofxAAMeter(x,y,w,h){

    alphaSlider = new ofxDatGuiSlider("alpha", 0.0, 1.0, 0.5);
    alphaSlider->onSliderEvent(this, &ofxAAMeter::onSliderEvent);
    
    tresholdSlider = new ofxDatGuiSlider("treshold", 0.0, 1.0, 0.5);
    tresholdSlider->onSliderEvent(this, &ofxAAMeter::onSliderEvent);
    
    setName("onset Test");
}

//------------------------------------
void ofxAAOnsetMeter::update(){
    
    onOffToggle->update();
    alphaSlider->update();
    tresholdSlider->update();

}
//------------------------------------
void ofxAAOnsetMeter::draw(){
    
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    
    
    //valueMeter-------------------------
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofFill();
    
    float meter_h;
    _onsetValue ? meter_h= -1.0*_h *0.5 : meter_h=0.0;
    ofDrawRectangle( _w * .25 , _h, _w * .5, meter_h);
    
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
    
    //on-off
    onOffToggle->setWidth(_w, 0.1);
    onOffToggle->setHeight(line_h * 0.75);
    onOffToggle->setPosition(_x, _y + line_h*1.75);
    onOffToggle->setLabelMargin(0.0);
    onOffToggle->draw();
    
    //alpha
    alphaSlider->setWidth(_w * 1.3, 0.0);
    alphaSlider->setHeight(line_h * 0.75);
    alphaSlider->setPosition(_x, _y + line_h*2.75);
    alphaSlider->setLabelMargin(0.0);
    alphaSlider->drawElemental();
    
    //treshold
    tresholdSlider->setWidth(_w * 1.3, 0.0);
    tresholdSlider->setHeight(line_h * 0.75);
    tresholdSlider->setPosition(_x, _y + line_h*3.75);
    tresholdSlider->setLabelMargin(0.0);
    tresholdSlider->drawElemental();


}