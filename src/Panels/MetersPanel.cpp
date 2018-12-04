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
#include "ofApp.h"
#include "ofxAAUtils.h"

#pragma mark - Core Funcs

void MetersPanel::setup(int x, int y, int w, int h){
    BasePanel::setup(x, y, w, h);
    
    guiView.setup(x, y, w, MT_GUI_COMP_HEIGHT, this);
    metersView.setup(x, guiView.maxY(), w, h - guiView.getHeight());
    
    _enabledAlgorithmTypes = ofxaa::allAvailableAlgorithmTypes;
}

void MetersPanel::setChannelAnalyzers(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    channelAnalyzers = chanAnalyzerPtrs;
    metersView.setupChannelMeters(chanAnalyzerPtrs, _enabledAlgorithmTypes);
}

void MetersPanel::setEnabledAlgorithms(vector<ofxAAAlgorithmType>& enabledAlgorithms) {
    _enabledAlgorithmTypes = enabledAlgorithms;
    metersView.setEnabledAlgorithms(enabledAlgorithms);
}

void MetersPanel::update(){
    metersView.update();
    guiView.update();
}

void MetersPanel::draw(){
    if (!View::mustDrawNewFrame()){
        View::drawLoadedTexture();
        return;
    }
    
    if (_isHidden){ return; }
    View::draw();
    TS_START("meters");
    metersView.draw();
    TS_STOP("meters");
    
    TS_START("gui-meters");
    guiView.draw();
    TS_STOP("gui-meters");
    
    View::loadViewInTexture();
}

void MetersPanel::exit(){
    metersView.exit();
}

void MetersPanel::keyPressed(int key){
    switch (key) {
        case '1':
            metersView.scrollUp();
            break;
        case '2':
            metersView.scrollDown();
            break;
    }
}

void MetersPanel::resize(int x, int y, int w, int h){
    View::resize(x, y, w, h);
    guiView.resize(x, y, w, MT_GUI_COMP_HEIGHT);
    metersView.resize(x, guiView.maxY(), w, h - guiView.getHeight());
}

#pragma mark - Settings

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
    ///gMaxFreq->setText(text);
    setAnalyzerMaxEstimatedValue(PITCH_FREQ, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-HFC", "");
    ///gMaxHfc->setText(text);
    setAnalyzerMaxEstimatedValue(HFC, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-CENTROID", "");
    ///gMaxCentroid->setText(text);
    setAnalyzerMaxEstimatedValue(CENTROID, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-SPEC-COMP", "");
    ///gMaxSpecComp->setText(text);
    setAnalyzerMaxEstimatedValue(SPECTRAL_COMPLEXITY, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-ROLLOFF", "");
    ///gMaxRollOff->setText(text);
    setAnalyzerMaxEstimatedValue(ROLL_OFF, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-ODD-EVEN", "");
    ///gMaxOddEven->setText(text);
    setAnalyzerMaxEstimatedValue(ODD_TO_EVEN, std::stof (text));
    //-----------------
    
    //-:Load channelPannels settings---------------------
    /**
    for(int i=0; i<metersView.channelPanels.size(); i++){
        string filenameChannelPanel = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        metersView.channelPanels[i]->loadSettingsFromFile(filenameChannelPanel);
    }
    */
}
//----------------------------------------------
void MetersPanel::saveSettings(string rootDir){
    
    //-:Save panel settings (gui) ------------------
    string filenamePanel = rootDir + _panelDir + "/meters_settings_gui.xml";
    ofxXmlSettings savedSettings;
    
    savedSettings.addTag("PANEL");
    savedSettings.pushTag("PANEL");
    /**
    savedSettings.addValue("MAX-FREQ",      gMaxFreq->getText());
    savedSettings.addValue("MAX-HFC",       gMaxHfc->getText());
    savedSettings.addValue("MAX-CENTROID",  gMaxCentroid->getText());
    savedSettings.addValue("MAX-SPEC-COMP", gMaxSpecComp->getText());
    savedSettings.addValue("MAX-ROLLOFF",   gMaxRollOff->getText());
    savedSettings.addValue("MAX-ODD-EVEN",  gMaxOddEven->getText());
    */
    savedSettings.saveFile(filenamePanel);
    
    //-:Save channelPannels settings---------------
    /**
    for(int i=0; i<metersView.channelPanels.size(); i++){
        string filenameChannelPanel = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        metersView.channelPanels[i]->saveSettingsToFile(filenameChannelPanel);
    
    }
    */
}
//----------------------------------------------
#pragma mark - Other funcs
//----------------------------------------------
void MetersPanel::reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    metersView.reset(chanAnalyzerPtrs);
}
//----------------------------------------------
bool MetersPanel::getFocused(){
    /**
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
    */
}

//--------------------------------------------------------------
void MetersPanel::setAnalyzerMaxEstimatedValue(ofxAAAlgorithmType algorithm, float value){
    for (ofxAudioAnalyzerUnit* anUnit : channelAnalyzers){
        anUnit->setMaxEstimatedValue(algorithm, value);
    }
}


