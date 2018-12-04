//
//  GuiView.cpp
//  Sonoscopio
//
//  Created by Leo on 20/09/2018.
//

#include "GuiView.h"

void GuiView::setup(int x, int y, int w, int h){
    View::setup(x, y, w, h);
    _bordCol = ofColor::grey;
    _bordWidth = 1;
    _guiCompHeight = h;
    setupGui();
}

void GuiView::setupGui() {
    createComponents();
    adjustComponentsSize();
}

void GuiView::update(){
    for(int i=0; i<_components.size(); i++){
        _components[i]->update();
    }
}

void GuiView::draw(){
    View::draw();
    for(int i=0; i<_components.size(); i++){
        _components[i]->draw();
    }
}

void GuiView::resize(int x, int y, int width, int height){
    View::resize(x, y, width, height);
    adjustComponentsSize();
}
