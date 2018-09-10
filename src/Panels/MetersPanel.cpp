/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * Sonoscopio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#include "MetersPanel.h"

//----------------------------------------------
#pragma mark - Core Funcs
//----------------------------------------------
void MetersPanel::setup(int x, int y, int w, int h){
    
    BasePanel::setup(x, y, w, h);
    metersView.setup(x, y + 25, w, h-25);
    metersView.setBackgroundColor(ofColor::orange);
    
    ///Borrar----------------
    _guiCompHeight = MT_GUI_COMP_HEIGHT;
    bordCol = ofColor::grey;
    bordWidth = 1;
    
    setBackgroundColor(ofColor::darkBlue);
    
    _panelDir = METERS_SETTINGS_DIR;
    
    
}
//----------------------------------------------
void MetersPanel::setChannelAnalyzers(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    channelAnalyzers = chanAnalyzerPtrs;
    metersView.setupChannelMeters(chanAnalyzerPtrs);
    
//    panelsNum = channelAnalyzers.size(); //Hace falta??
   
//    int panelHeight = (_h - _guiCompHeight) / panelsNum;
//
//    for (int i=0; i<panelsNum; i++){
//        int y_pos = _y + panelHeight*i;
//        int panelId = i;
//        ChannelMetersView * p = new ChannelMetersView(_x, y_pos, _w, panelHeight, panelId, channelAnalyzers[i]);
//
//        if(i%2) p->setMainColor(panelColor2);
//        else p->setMainColor(panelColor1);
//
//        channelPanels.push_back(p);
//    }
    setupGui();
    
}
//----------------------------------------------
void MetersPanel::update(){
    

    metersView.update();

    
    //update gui components
    for(int i=0; i<components.size(); i++){
        components[i]->update();
    }
}


//----------------------------------------------
void MetersPanel::draw(){
    if (_isHidden){ return; }
    
    View::draw();
    metersView.draw();
    return;

    for(int i=0; i<components.size(); i++){
        components[i]->draw();
    }

}
//----------------------------------------------
void MetersPanel::exit(){
    metersView.exit();
//    for (ChannelMetersView* p : channelPanels){
//        p->exit();
//    }
//    channelPanels.clear();

}
//----------------------------------------------
#pragma mark - Settings
//----------------------------------------------

void MetersPanel::loadSettings(string rootDir){
    
    //string fileName = workingDir+"/"+"metersSettings.xml";
    
    //-:Load panel settings (gui)---------------------
    ofxXmlSettings xml;
    string filenamePanel = rootDir + _panelDir + "/meters_settings_gui.xml";
    
    if( xml.loadFile(filenamePanel) ){
        ofLogVerbose()<<"MainPanel: "<< filenamePanel <<" loaded.";
    }else{
        ofLogError()<< "MainPanel: unable to load " << filenamePanel ;
        return;
    }
    //-----------
    string text = xml.getValue("PANEL:MAX-FREQ", "");
    gMaxFreq->setText(text);
    setAnalyzerMaxEstimatedValue(PITCH_FREQ, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-HFC", "");
    gMaxHfc->setText(text);
    setAnalyzerMaxEstimatedValue(HFC, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-CENTROID", "");
    gMaxCentroid->setText(text);
    setAnalyzerMaxEstimatedValue(CENTROID, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-SPEC-COMP", "");
    gMaxSpecComp->setText(text);
    setAnalyzerMaxEstimatedValue(SPECTRAL_COMPLEXITY, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-ROLLOFF", "");
    gMaxRollOff->setText(text);
    setAnalyzerMaxEstimatedValue(ROLL_OFF, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-ODD-EVEN", "");
    gMaxOddEven->setText(text);
    setAnalyzerMaxEstimatedValue(ODD_TO_EVEN, std::stof (text));
    //-----------------
    
    //-:Load channelPannels settings---------------------
    for(int i=0; i<metersView.channelPanels.size(); i++){
        string filenameChannelPanel = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        metersView.channelPanels[i]->loadSettingsFromFile(filenameChannelPanel);
    }
}
//----------------------------------------------
void MetersPanel::saveSettings(string rootDir){
    
    //-:Save panel settings (gui) ------------------
    string filenamePanel = rootDir + _panelDir + "/meters_settings_gui.xml";
    ofxXmlSettings savedSettings;
    
    savedSettings.addTag("PANEL");
    savedSettings.pushTag("PANEL");
    savedSettings.addValue("MAX-FREQ",      gMaxFreq->getText());
    savedSettings.addValue("MAX-HFC",       gMaxHfc->getText());
    savedSettings.addValue("MAX-CENTROID",  gMaxCentroid->getText());
    savedSettings.addValue("MAX-SPEC-COMP", gMaxSpecComp->getText());
    savedSettings.addValue("MAX-ROLLOFF",   gMaxRollOff->getText());
    savedSettings.addValue("MAX-ODD-EVEN",  gMaxOddEven->getText());
    
    savedSettings.saveFile(filenamePanel);
    
    //-:Save channelPannels settings---------------
    for(int i=0; i<metersView.channelPanels.size(); i++){
        string filenameChannelPanel = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        metersView.channelPanels[i]->saveSettingsToFile(filenameChannelPanel);
    }
}
//----------------------------------------------
#pragma mark - Other funcs
//----------------------------------------------
void MetersPanel::reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    metersView.reset(chanAnalyzerPtrs);
}

//----------------------------------------------

void MetersPanel::resize(int x, int y, int w, int h){
    
    View::resize(x, y, w, h);
    
    metersView.resize(x, y, w, h);
    
    adjustPosAndHeight(y, h);
    adjustGuiSize(_y, _w, _h);
}
//----------------------------------------------
void MetersPanel::adjustPosAndHeight(int y, int h){
    
//    int panelHeight = (_h - _guiCompHeight) / panelsNum;
//
//    for(int i=0; i<channelPanels.size(); i++){
//        int y_pos = _y + panelHeight*i;
//        channelPanels[i]->adjustPosAndHeight(y_pos, panelHeight);
//    }
    
}

//----------------------------------------------
void MetersPanel::setupGui(){
    
    if(channelAnalyzers[0] == NULL){
        ofLogError()<<"Meters Panel: Channel Analyzer is null, cant setupGui.";
        return;
    }
    
    ofxDatGuiComponent* component;
    
    component = new ofxDatGuiButton("FULL DISPLAY");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MetersPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    
    gMaxFreq = new ofxDatGuiTextInput("MAX FREQ",
                                      ofToString(channelAnalyzers[0]->getMaxEstimatedValue(PITCH_FREQ)));
    component = gMaxFreq;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    
    gMaxHfc = new ofxDatGuiTextInput("MAX HFC",
                                     ofToString(channelAnalyzers[0]->getMaxEstimatedValue(HFC)));
    component = gMaxHfc;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxCentroid = new ofxDatGuiTextInput("MAX CENTROID",
                                          ofToString(channelAnalyzers[0]->getMaxEstimatedValue(CENTROID)));
    component = gMaxCentroid;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxSpecComp = new ofxDatGuiTextInput("MAX SPEC-COMP",
                                          ofToString(channelAnalyzers[0]->getMaxEstimatedValue(SPECTRAL_COMPLEXITY)));
    component = gMaxSpecComp;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxRollOff = new ofxDatGuiTextInput("MAX ROLL-OFF",
                                         ofToString(channelAnalyzers[0]->getMaxEstimatedValue(ROLL_OFF)));
    component = gMaxRollOff;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxOddEven = new ofxDatGuiTextInput("MAX ODD-EVEN",
                                         ofToString(channelAnalyzers[0]->getMaxEstimatedValue(ODD_TO_EVEN)));
    component = gMaxOddEven;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);

    adjustGuiSize(_y, _w, _h);
    
}
//----------------------------------------------
void MetersPanel::adjustGuiSize(int y, int w, int h){
    
    int guiCompWidth = _w / 7;
    
    int gui_y = _y + _h - _guiCompHeight;
    int gui_x = _x;
    
    ofxDatGuiComponent* component;
    //FULL DISPLAY
    component = components[0];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.35);//width x2
    
    //MAX PITCH FREQ
    gui_x += guiCompWidth;
    component = components[1];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX HFC
    gui_x += guiCompWidth;
    component = components[2];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX CENTROID
    gui_x += guiCompWidth;
    component = components[3];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX SPEC COMP
    gui_x += guiCompWidth;
    component = components[4];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX ROLL-OFF
    gui_x += guiCompWidth;
    component = components[5];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX ODD TO EVEN
    gui_x += guiCompWidth;
    component = components[6];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);

    
}
//----------------------------------------------
bool MetersPanel::getFocused(){
    if(gMaxFreq->getFocused() ||
       gMaxHfc->getFocused() ||
       gMaxCentroid->getFocused()  ||
       gMaxSpecComp->getFocused()  ||
       gMaxRollOff->getFocused()  ||
       gMaxOddEven->getFocused()   )
    {
        return true;
    }else{
        return false;
    }
}

//--------------------------------------------------------------
void MetersPanel::setAnalyzerMaxEstimatedValue(ofxAAAlgorithmType algorithm, float value){
    
    for (ofxAudioAnalyzerUnit* anUnit : channelAnalyzers){
        anUnit->setMaxEstimatedValue(algorithm, value);
    }
    
}
//--------------------------------------------------------------
#pragma mark - Gui listeners
//--------------------------------------------------------------

void MetersPanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if(e.target->getLabel()=="FULL DISPLAY"){
        metersView.toggleFullDisplay();
    }
}

//--------------------------------------------------------------
void MetersPanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    //cout << "onButtonEvent: " << e.text << endl;
    //cout << "onTextInput: " << e.text << endl;
    float inputValue = 0.0;
    try{
        inputValue = std::stof (e.text) ;
    }
    catch (const std::invalid_argument& ia) {
        e.target->setText("ERROR");
        std::cerr << "Invalid Value: " << ia.what() << '\n';
        return;
    }
    
    if (e.target->getLabel()=="MAX FREQ"){
        setAnalyzerMaxEstimatedValue(PITCH_FREQ, inputValue);
    }else if (e.target->getLabel()=="MAX HFC"){
        setAnalyzerMaxEstimatedValue(HFC, inputValue);
    }else if (e.target->getLabel()=="MAX CENTROID"){
        setAnalyzerMaxEstimatedValue(CENTROID, inputValue);
    }else if (e.target->getLabel()=="MAX SPEC-COMP"){
        setAnalyzerMaxEstimatedValue(SPECTRAL_COMPLEXITY, inputValue);
    }else if (e.target->getLabel()=="MAX ROLL-OFF"){
        setAnalyzerMaxEstimatedValue(ROLL_OFF, inputValue);
    }else if (e.target->getLabel()=="MAX ODD-EVEN"){
        setAnalyzerMaxEstimatedValue(ODD_TO_EVEN, inputValue);
    }
    
    ofLogVerbose()<<"Max value Set: "<<e.target->getLabel()<<" = " << inputValue;
        
}

