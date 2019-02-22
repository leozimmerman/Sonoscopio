//
//  FileManager.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 06/12/2018.
//
#include "ofApp.h"
#include "FileManager.h"
#include "SettingsManager.h"
#include "AnalysisDataSaver.h"

FileManager::FileManager(){
    fileName = "";
    bFileLoaded = false;
}

void FileManager::openFileDialog(){

    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a wav or mp3 file");

    if (openFileResult.bSuccess){
        processFileSelection(openFileResult);
    }else {
        ofLogVerbose("User pressed CANCEL");
    }
}

void FileManager::processFileSelection(ofFileDialogResult openFileResult){
    
    fileName = openFileResult.getName();
    absolutePath = openFileResult.getPath();
    
    ofFile file(absolutePath);
    directory = file.getEnclosingDirectory();
    baseName = file.getBaseName();
    
    if (file.exists()){
        string fileExtension = ofToUpper(file.getExtension());
        if (fileExtension == WAV_EXTENSION || fileExtension == MP3_EXTENSION) {
            openAudioFile(absolutePath);
        }else{
            string message = "File extension not compatible";
            ofNotifyEvent(ofApp::errorEvent, message);
        }
    }
}

void FileManager::openAudioFile(string filename){
    timelinePanelPtr->openAudioFile(filename);
    if (timelinePanelPtr->isFileLoaded()){
        bFileLoaded = true;
        samplerate = timelinePanelPtr->getAudioFileSampleRate();
        channels = timelinePanelPtr->getAudioFileNumChannels();
        duration = timelinePanelPtr->getAudioFileDuration();
        
        int durationInFrames = timelinePanelPtr->getTotalFramesNum();
        int framerate = timelinePanelPtr->getFrameRate();
        mainPanelPtr->setAudioFileInfo(baseName, directory, duration, samplerate, channels, durationInFrames, framerate);
        
        timelinePanelPtr->setEnabled(true);
        metersPanelPtr->setEnabled(true);
        
        metersPanelPtr->resetAnalyzer(samplerate);
        SettingsManager::getInstance().loadSettings();
        AnalysisDataSaver::getInstance().reset();
    }
}



