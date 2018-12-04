//
//  GuiFactory.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 04/12/2018.
//

#pragma once

#include "ofxDatGui.h"

namespace GuiFactory {
    
    static ofColor borderColor = ofColor::grey;
    static int borderWidth = 1;
    
    template<typename T, typename args, class ListenerClass>
    ofxDatGuiButton* createButton(string label, T* owner, void (ListenerClass::*listenerMethod)(args)){
        
        ofxDatGuiButton* button =  new ofxDatGuiButton(label);
        button->onButtonEvent(owner, listenerMethod);
        button->setLabelAlignment(ofxDatGuiAlignment::LEFT);
        button->setBorder(borderColor, borderWidth);
        button->setBorderVisible(TRUE);
        button->setStripeVisible(false);
        return button;
    }
    
    template<typename T, typename args, class ListenerClass>
    ofxDatGuiToggle* createToggle(string label, bool enabled, T* owner, void (ListenerClass::*listenerMethod)(args)){
        
        ofxDatGuiToggle* toggle = new ofxDatGuiToggle(label, enabled);
        toggle->setLabelAlignment(ofxDatGuiAlignment::LEFT);
        toggle->onButtonEvent(owner, listenerMethod);
        toggle->setBorder(borderColor, borderWidth);
        toggle->setBorderVisible(TRUE);
        toggle->setStripeVisible(false);
        return toggle;
    }
    
    
    template<typename T, typename args, class ListenerClass>
    ofxDatGuiSlider* createSlider(string label, float min, float max, double val, T* owner, void (ListenerClass::*listenerMethod)(args)){
        
        ofxDatGuiSlider* slider = new ofxDatGuiSlider(label, min, max, val);
        slider->onSliderEvent(owner, listenerMethod);
        slider->setBorder(borderColor, borderWidth);
        slider->setBorderVisible(TRUE);
        slider->setStripeVisible(false);
        return slider;
    }
    
    template<typename T, typename args, class ListenerClass>
    ofxDatGuiTextInput* createTextInput(string label, string placeholder, T* owner, void (ListenerClass::*listenerMethod)(args)){
        
        ofxDatGuiTextInput* textInput = new ofxDatGuiTextInput(label, placeholder);
        textInput->setLabelAlignment(ofxDatGuiAlignment::LEFT);
        textInput->onTextInputEvent(owner, listenerMethod);
        textInput->setBorder(borderColor, borderWidth);
        textInput->setBorderVisible(TRUE);
        textInput->setStripeVisible(false);
        return textInput;
    }
    
    template<typename T, typename args, class ListenerClass>
    ofxDatGuiDropdown* createDropDown(string label, const vector<string>& options, T* owner, void (ListenerClass::*listenerMethod)(args)){
       
        ofxDatGuiDropdown* dropdown = new ofxDatGuiDropdown(label, options);
        dropdown->onDropdownEvent(owner, listenerMethod);;
        dropdown->setBorder(borderColor, borderWidth);
        dropdown->setBorderVisible(TRUE);
        dropdown->setStripeVisible(false);
        dropdown->setStripeColor(borderColor);
        return dropdown;
    }
}
