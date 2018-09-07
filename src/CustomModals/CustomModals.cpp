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
    
    setTitle("CONFIGURATION");
    
    getButton(0)->setLabel("APPLY");
    getButton(0)->onButtonEvent(this, &MenuModal::onApplyButtonEvent);
    
    addButton("CANCEL");
    
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
    
    
//    autoSize();
    
    
}

void MenuModal::setMainAppPtr(ofBaseApp* appPtr){
    menuMainAppPtr = dynamic_cast<ofApp*>(appPtr);
}

void MenuModal::display(int height){
    //TODO: Remove this reference to config
    int fps = menuMainAppPtr->config.getFrameRate();
    string host = menuMainAppPtr->config.osc().host;
    int port = menuMainAppPtr->config.osc().port;
    int bpm = menuMainAppPtr->getBpm();
    int bufferSize = menuMainAppPtr->config.getBufferSize();
    
    gFps->setText(std::to_string(fps));
    gHost->setText(host);
    gPort->setText(std::to_string(port));
    gBpm->setText(std::to_string(bpm));
    selectedBufferSize = bufferSize;
    
    setHeight(height);
    
    show();
    
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
        menuMainAppPtr->timePanel.timelineView.timeline.setNewBPM( bpm );
        menuMainAppPtr->oscSender.setHost(host);
        menuMainAppPtr->oscSender.setPort(port);
        
        
    } catch (const std::invalid_argument& ia) {
        //e.target->setText("ERROR");
        std::cerr << "Invalid FPS: " << ia.what() << '\n';
    }
    
}


void MenuModal::onTextInputEvent(ofxDatGuiTextInputEvent e){

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
