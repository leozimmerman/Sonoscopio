//
//  CustomModals.cpp
//  Sonoscopio
//
//  Created by Leo on 12/15/17.
//

#include "CustomModals.h"
#include "ofApp.h"

ofApp* menuMainAppPtr;

MenuModal::MenuModal(){
    
    setTitle("CONIGURATION");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MenuModal::onApplyButtonEvent);
    
    ofxDatGuiComponent* component;
    //SET FRAMERATE
    gFps = new ofxDatGuiTextInput("SET FPS", ofToString(INIT_FPS));
    component = gFps;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MenuModal::onTextInputEvent);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    //component->setLabelMargin(20.0);
    addComponent(component);
    
    gBpm = new ofxDatGuiTextInput("BPM", "120");
    component = gBpm;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MenuModal::onTextInputEvent);
    //component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    addComponent(component);
    
    gHost = new ofxDatGuiTextInput("HOST", "localhost");
    component = gHost;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MenuModal::onTextInputEvent);
    //component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    addComponent(component);
    
    gPort = new ofxDatGuiTextInput("PORT", "12345");
    component = gPort;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MenuModal::onTextInputEvent);
    //component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    addComponent(component);
    
    vector<string> buff_sizes = {"256", "512", "1024", "2048"};
    gBufferSize = new ofxDatGuiDropdown("BUFFER SIZE", buff_sizes);
    component = gBufferSize;
    component->onDropdownEvent(this, &MenuModal::onBufferSizeDropdownEvent);
    //component->onButtonEvent(this, &MenuModal::onButtonEvent);
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    //component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    addComponent(component);
    
    
    addButton("CANCEL");
    
//    autoSize();
    
    
}

void MenuModal::setMainAppPtr(ofBaseApp* appPtr){
    menuMainAppPtr = dynamic_cast<ofApp*>(appPtr);
}

void MenuModal::display(){
    
    int fps = menuMainAppPtr->getFrameRate();
    string host = menuMainAppPtr->getOscHost();
    int port = menuMainAppPtr->getOscPort();
    int bpm = menuMainAppPtr->getBpm();
    int bufferSize = menuMainAppPtr->getBufferSize();
    
    gFps->setText(std::to_string(fps));
    gHost->setText(host);
    gPort->setText(std::to_string(port));
    gBpm->setText(std::to_string(bpm));
    selectedBufferSize = bufferSize;
    
    show();
    
    cout<<"MENU MODAL DISPLAY"<<endl;
}

void MenuModal::applyConfiguration(){
    
    int fps;
    string host;
    int port;
    float bpm;
    int bufferSize;
    
    try {
        
        fps = std::stoi(gFps->getText());
        host = gHost->getText();
        port = std::stoi(gPort->getText());
        bpm = std::stof(gBpm->getText());
        bufferSize = selectedBufferSize;
        
        cout << "APPLYING: " << fps <<" " <<  host <<" " <<  port <<" " <<bpm << " " << bufferSize << endl;
        
        menuMainAppPtr->setFrameRate( fps );
        menuMainAppPtr->setBufferSize(bufferSize);
        menuMainAppPtr->timePanel.timeline.setNewBPM( bpm );
        menuMainAppPtr->setOscSenderHost(host);
        menuMainAppPtr->setOscSenderPort( port );
        
        
    } catch (const std::invalid_argument& ia) {
        //e.target->setText("ERROR");
        std::cerr << "Invalid FPS: " << ia.what() << '\n';
    }
    
}


void MenuModal::onTextInputEvent(ofxDatGuiTextInputEvent e){
//    //cout << "onButtonEvent: " << e.text << endl;
//    //cout << "onTextInput: " << e.text << endl;
//    if (e.target->getLabel()=="BPM"){
//        try{
//           /// mMainAppPtr->timePanel.timeline.setNewBPM( std::stof (e.text) );
//        }
//        catch (const std::invalid_argument& ia) {
//            e.target->setText("ERROR");
//            std::cerr << "Invalid BPM: " << ia.what() << '\n';
//        }
//    }else if (e.target->getLabel()=="HOST"){
//
//        ///mMainAppPtr->setOscSenderHost(e.text);
//
//    }else if (e.target->getLabel()=="PORT"){
//        try{
//           /// mMainAppPtr->setOscSenderPort( std::stoi(e.text) );
//        }
//        catch (const std::invalid_argument& ia) {
//            e.target->setText("ERROR");
//            std::cerr << "Invalid PORT: " << ia.what() << '\n';
//        }
//    } else if (e.target->getLabel()=="SET FPS"){
//        try{
//            ///mMainAppPtr->setFrameRate( std::stoi(e.text) );
//        }
//        catch (const std::invalid_argument& ia) {
//            e.target->setText("ERROR");
//            std::cerr << "Invalid FPS: " << ia.what() << '\n';
//        }
//    }
    
}

//--------------------------------------------------------------
bool MenuModal::getFocused(){
    if(gHost->getFocused() ||
       gPort->getFocused() ||
       gBpm->getFocused()  ||
       gFps->getFocused()   )
    {
        return true;
    }else{
        return false;
    }
}

//--------------------------------------------------------------
void MenuModal::onApplyButtonEvent(ofxDatGuiButtonEvent e) {
    applyConfiguration();
    hide();
}
//--------------------------------------------------------------
void MenuModal::onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e)
{
    // ofLogVerbose() << "onDropdownEvent: " << e.child << "--"<<e.target->getLabel()<<"--"<<e.parent;
    
    switch (e.child) {
            
        case 0:
            selectedBufferSize = 256;
            break;
        case 1:
            selectedBufferSize = 512;
            break;
        case 2:
            selectedBufferSize = 1024;
            break;
        case 3:
            selectedBufferSize = 2048;
            break;
            
        default:
            break;
    }
    
}
