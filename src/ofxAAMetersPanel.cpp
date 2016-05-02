
#include "ofxAAMetersPanel.h"

void ofxAAMetersPanel::setup(int x, int y, int width, int height){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    
    setBackgroundColor(ofColor::darkorange);
    
    meter = new ofxAAMeter(_x+30, _y, 50, _h );
    
    binMeter = new ofxAABinMeter(_x+70, _y, 200, _h );
    binMeter->setName("binTest");
    
    onsetMeter = new ofxAAOnsetMeter(_x+500, _y, 200, _h );
    
}
//------------------------------------------------
void ofxAAMetersPanel::update(){
    meter->update();
    binMeter->update();
    onsetMeter->update();
    
    
    //binMeter noise simulation
    vector<float> testValues;
    testValues.assign(200, 0.0);
    for (int i=0; i<testValues.size(); i++){
        testValues[i] = ofNoise(ofGetElapsedTimef(), i);
    }
    binMeter->setNumBins(testValues.size());
    binMeter->setValues(testValues);

}
//------------------------------------------------
void ofxAAMetersPanel::draw(){
    
    //background
    ofPushStyle();
        ofSetColor(getBackgroundColor());
        ofDrawRectangle(_x, _y, _w, _h);
    ofPopStyle();
    
    meter->setValue(ofNoise(ofGetElapsedTimef()));
    meter->draw();
    
    binMeter->setValue(ofNoise(ofGetElapsedTimef()));
    binMeter->draw();
    
    onsetMeter->setValue(TRUE);
    onsetMeter->draw();

}
//------------------------------------------------
void ofxAAMetersPanel::adjustPosAndHeight(int y, int h){
    
    _y = y;
    _h = h;
    
    meter->setPosAndSize(_x+30, _y, 50, _h);
    binMeter->setPosAndSize(_x+200, _y, 200, _h);
    onsetMeter->setPosAndSize(_x+500, _y, 200, _h );
    
}

