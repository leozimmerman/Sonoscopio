//
//  GuiView.hpp
//  Sonoscopio
//
//  Created by Leo on 06/09/2018.
//

#pragma once
#include "ofMain.h"
#include "View.h"

class GuiView : public View {
    void setup();
    void update();
    void draw() override;
};
