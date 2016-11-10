
#pragma once

#include "ofMain.h"

#include "ofxAAMeter.h"


class ofxAABinMeter : public ofxAAMeter{

 public:

    ofxAABinMeter(string name, int x, int y, int w, int h, int panelId);
    
    void draw();
    
    void drawMeter();
    void drawLabel();
    
    void setBinsNum(int bins){_nBins = bins;}
    void setValues(vector<float>& vals){_values = vals;}
    
    //void updateColors();
    void updateComponentsPositions();
    void updateComponentsWidth();
    
    void setPosAndSize(int x, int y, int w, int h);
    
    vector<float>& getValues(){return _values;}
    
 protected:
    
    vector<float> _values;
    int _nBins;
};