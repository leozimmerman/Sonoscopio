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

#include "AnalysisDataSaver.h"
#include "FileManager.h"

AnalysisDataSaver::AnalysisDataSaver(){
    bSaveVectorValues = TRUE;
    verdana.load("gui_assets/fonts/verdana.ttf", 25, false, false);
}

void AnalysisDataSaver::reset(){
    
    percentage = 0.0;
    
    soundfilePath = FileManager::getInstance().getFilePath();
    durationSecs = FileManager::getInstance().getFileDuration();
    sampleRate = FileManager::getInstance().getFileSampleRate();
    
    if (timelinePanelPtr != NULL){
        frameRate = timelinePanelPtr->getFrameRate();
        totalFramesNum = timelinePanelPtr->getTotalFramesNum();
    }

    if (metersPanelPtr != NULL){
        bufferSize = metersPanelPtr->getBufferSize();
        channels = metersPanelPtr->getChannelsNum();
    }
}
//------------------------------------
void AnalysisDataSaver::updateFrameRate(int fps, int framesNum){
    frameRate       = fps;
    totalFramesNum  = framesNum;
}
//------------------------------------
void AnalysisDataSaver::start(){
    // Mutex blocking is set to true by default
    // It is rare that one would want to use startThread(false).
    startThread();
    ofLogVerbose("AnalysisDataSaver: ")<<"thread started.";
}
//------------------------------------
void AnalysisDataSaver::stop(){
    stopThread();
    ofLogVerbose("AnalysisDataSaver: ")<<"thread stopped.";
}

bool AnalysisDataSaver::drawSavingAnalysisSign(){
    
    if (!isThreadRunning()){
        return false;
    }
    ofPushStyle();
    ofFill();
    ofSetColor(0,150);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255);
    
    string displayStr = "Saving Analysis Data...  " + ofToString(percentage, 2) + "%";
    
    //align center
    int label_w = verdana.stringWidth(displayStr);
    int label_x =  ofGetWidth() * .5 - label_w *.5;
    
    verdana.drawString(displayStr, label_x , ofGetHeight()/2);
    ofPopStyle();
    
    return true;
}

//------------------------------------
void AnalysisDataSaver::threadedFunction(){
    
    ofSoundBuffer frameSoundBuffer;
    
    while(isThreadRunning()){
        if(lock()){
            float initTime = ofGetElapsedTimef();
            ofxXmlSettings savedSettings;
            //FILE-DATA-----------------
            savedSettings.addTag("FILE-INFO");
            savedSettings.pushTag("FILE-INFO");
            savedSettings.addValue("soundfile", soundfilePath);
            savedSettings.addValue("frameRate", frameRate);
            savedSettings.addValue("totalFramesNum", totalFramesNum);
            savedSettings.addValue("durationSeconds", durationSecs);
            savedSettings.addValue("sampleRate", sampleRate);
            savedSettings.addValue("bufferSize", bufferSize);
            savedSettings.addValue("channelsNum", channels);
            savedSettings.popTag();//pop from FILE-INFO back into MAIN
            //----------------------------------------
            savedSettings.addTag("ANALYSIS-DATA");
            savedSettings.pushTag("ANALYSIS-DATA");
            //FRAME LOOP
            for (int frameNum=0; frameNum<totalFramesNum; frameNum++){
                percentage = (frameNum / (float)totalFramesNum) * 100.0;
                ofLogVerbose("AnalysisDataSaver threaded Function: ")<<"saving frame: "<<frameNum<<endl;
                string frameTag = "FRAME-" + ofToString(frameNum);
                savedSettings.addTag(frameTag);
                savedSettings.pushTag(frameTag);
                //ANALYZER-------------------------------------
                savedSettings.addTag("ANALYZER");
                savedSettings.pushTag("ANALYZER");
                //analyze buffer for frame:
                frameSoundBuffer = timelinePanelPtr->getSoundBufferMonoForFrame(frameNum, bufferSize);//mono soundbuffer
                metersPanelPtr->analyzeBuffer(frameSoundBuffer);
                metersPanelPtr->update();
                //-:Get Meters values--------------------------------
                vector<std::map<string, float>> metersValues = metersPanelPtr->getMetersValues();
                
                if(metersValues.size() != channels){
                    ofLogError("datasSaver threaded function:")<< "metersPanel channels number incorrect.";
                }
                for(int i=0; i<metersValues.size(); i++){
                    //"i" -> channel
                    string channelTag = "CHANNEL-" + ofToString(i);
                    savedSettings.addTag(channelTag);
                    savedSettings.pushTag(channelTag);
                    
                    for (auto const& map : metersValues[i]){
                        auto key = map.first;
                        auto value = map.second;
                        savedSettings.addValue(key, value);
                    }
                    
                    if(bSaveVectorValues){
                        
                        vector<std::map<string, vector<float>>> metersVectorValues = metersPanelPtr->getMetersVectorValues();
                        
                        if(metersVectorValues.size() != metersValues.size()){
                            ofLogError("datasSaver threaded function:")<<"metersValues and metersVectorValues sizes not matching.";
                            return;
                        }
                        for (auto const& map : metersVectorValues[i]){
                            auto key = map.first;
                            savedSettings.addTag(key);
                            savedSettings.pushTag(key);
                            for (int j=0; j<metersVectorValues[i].at(key).size(); j++){
                                float val = metersVectorValues[i].at(key)[j];
                                savedSettings.addValue(ofToString(j), val);
                            }
                            savedSettings.popTag();
                        }
                    }
                    savedSettings.popTag();//pop from CHANNEL-i back into ANALYZER
                }
                savedSettings.popTag();//pop from ANALYZER back into frameTag
        
                //TIMELINE-----------------------------------------
                savedSettings.addTag("TIMELINE");
                savedSettings.pushTag("TIMELINE");
                timelinePanelPtr->setCurrentFrame(frameNum);
                std::map<string, float> timelineValues = timelinePanelPtr->getTracksValues();
                
                for (auto const& map : timelineValues){
                    string key = map.first;
                    float floatValue = map.second;
                    savedSettings.addValue(key, floatValue);
                }
                savedSettings.popTag();//pop from TIMELINE back into frameTag
                savedSettings.popTag();//pop from frameTag back into ANALYSIS-DATA
            }
            timelinePanelPtr->setCurrentFrame(0);
            //save and stop----------------------------------
            string fileName = FileManager::getInstance().getAnalysisFileName();
            savedSettings.saveFile(fileName);
            unlock();
            stop();
            //--------------------------
            float finalTime = ofGetElapsedTimef();
            ofLogVerbose("threadedFunction took: ")<<finalTime-initTime<<" secs.";
        }
        else{
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            
            percentage = 0.0;
        }
    }

    
}

