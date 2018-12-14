//
//  GuiFactory.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 13/12/2018.
//

#include "GuiFactory.h"

ofxDatGuiValuePlotter* GuiFactory::createValuePlotter(string label, float min, float max){
    ofxDatGuiValuePlotter* valuePlotter = new ofxDatGuiValuePlotter(label, 0.0, 1.0);
    valuePlotter->setSpeed(1.0);
    valuePlotter->setLabelMargin(0.0);
    valuePlotter->setBorderVisible(TRUE);
    valuePlotter->setStripeVisible(false);
    return valuePlotter;
}

