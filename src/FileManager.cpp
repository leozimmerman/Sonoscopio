//
//  FileManager.cpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 06/12/2018.
//

#include "FileManager.h"
#include "SettingsManager.h"

FileManager::FileManager(){
    fileName = "";
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
            //TODO: Show modal
            ofLogVerbose("File extension not compatible");
        }
    }
}

void FileManager::openAudioFile(string filename){
    timelinePanelPtr->openAudioFile(filename);
    if (timelinePanelPtr->isFileLoaded()){
        samplerate = timelinePanelPtr->getAudioFileSampleRate();
        channels = timelinePanelPtr->getAudioFileNumChannels();
        duration = timelinePanelPtr->getAudioFileDuration();
        
        mainPanelPtr->setAudioFileInfo(filename, directory, duration, samplerate, channels, 0, 0);
        metersPanelPtr->resetAnalyzer(samplerate);
        SettingsManager::getInstance().loadSettings();
        ///dataSaver.reset();
    }
}



