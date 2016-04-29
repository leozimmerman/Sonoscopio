
#include "ofxAAMeter.h"

ofxAAMeter::ofxAAMeter(int x, int y, int w, int h){
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _drawRect.set(x, y, w, h);
    
    _backgroundColor.set(ofColor::black);
    _mainColor.set(ofColor::white);
    
    verdana.load("verdana.ttf", 10, true, true);
    verdana.setLineHeight(18.0f);
    verdana.setLetterSpacing(1.037);
    
    _name = "Centroid";
    
    
    smoothSlider = new ofxDatGuiSlider("slider", 0.0, 1.0, 0.5);
    smoothSlider->onSliderEvent(this, &ofxAAMeter::onSliderEvent);
    
    
    onOffToggle = new ofxDatGuiToggle("on", TRUE);
    onOffToggle->onButtonEvent(this, &ofxAAMeter::onButtonEvent);
    onOffToggle->setStripeVisible(false);
    //onOffToggle->setBorderVisible(false);
    onOffToggle->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    

   
   
    
}
//------------------------------------------------
void ofxAAMeter::update(){
    //update gui components
    smoothSlider->update();
    onOffToggle->update();
    

}
//------------------------------------------------
void ofxAAMeter::draw(){
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    
    
    //valueMeter-------------------------
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofFill();
    float meter_h = -1 * (_h * _value);
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
    
    //on-off
    
    onOffToggle->setWidth(_w, 0.1);
    onOffToggle->setHeight(line_h * 0.75);
    onOffToggle->setPosition(_x, _y + line_h*1.75);
    onOffToggle->setLabelMargin(0.0);
    onOffToggle->draw();
    
    //smooth
    
    smoothSlider->setWidth(_w * 1.3, 0.0);
    smoothSlider->setHeight(line_h * 0.75);
    smoothSlider->setPosition(_x, _y + line_h*2.75);
    onOffToggle->setLabelMargin(0.0);
    smoothSlider->drawElemental();
    
  
    ///all set Pos in draw()???
    
    
   
    
    ofPopStyle();
}
//------------------------------------------------
void ofxAAMeter::setPosAndSize(int x, int y, int w, int h){
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _drawRect.set(x, y, w, h);
}
//------------------------------------------------
void ofxAAMeter::setHeight(float h){
    _h = h;
    _drawRect.setHeight(h);
}
//------------------------------------------------
void ofxAAMeter::onSliderEvent(ofxDatGuiSliderEvent e){
    cout << _name << "- smooth: "<<e.value << endl;
}
//------------------------------------------------

void ofxAAMeter::onButtonEvent(ofxDatGuiButtonEvent e){
   cout << _name << "- on-off: "<<e.enabled << endl;
}



