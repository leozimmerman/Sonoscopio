//
//  TextModal.h
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 30/11/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxModal.h"


class TextModal : public ofxModalWindow {
public:
    TextModal(){
        setTitle("Text");
        getButton(0)->setLabel("OK");
        setTheme(mTheme);
        setCancelable(false);
        setMessage("This is an alert message!");
        
    }
    
    void display(string title, string message)
    {
        setTitle(title);
        setMessage(message);
        ofxModalWindow::show();
    }
};
