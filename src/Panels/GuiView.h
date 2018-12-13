//
//  GuiView.hpp
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#pragma once
#include "ofxDatGui.h"
#include "View.h"

class GuiView : public View {
public:
    void setup(int x, int y, int w, int h) override;
    void update();
    void draw() override;
    
    void resize(int x, int y, int width, int height) override;
    void setupGui();
    
    virtual void createComponents() =0;
    virtual void adjustComponentsSize() =0;
    
    virtual bool getFocused() =0;
    
protected:
    vector<ofxDatGuiComponent*> _components;
    bool _isHidden = false;
    ofColor _bordCol;
    int _bordWidth;
    int _guiCompHeight;
    int  _guiCompWidth;
    
};
