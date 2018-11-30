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

#include "ofApp.h"

ofApp* sMainApp;
//------------------------------------
void AnalysisDataSaver::setup(ofBaseApp* appPtr){
    
    sMainApp = dynamic_cast<ofApp*>(appPtr);
    
    bSaveVectorValues = TRUE;
    
    reset();
    
}
//------------------------------------
void AnalysisDataSaver::reset(){
    
    if(sMainApp==NULL){
        ofLogError("AnalysisDataSaver: ") <<"mainApp ptr not assigned. Need to call setup function first.";
        return;
    }
    
    soundfilePath = sMainApp->getSoundfilePath();
    //TODO: Remove this reference to config
    frameRate = sMainApp->config.getFrameRate();
    totalFramesNum = sMainApp->getTotalFramesNum();
    durationSecs = sMainApp->getDurationInSeconds();
    
    sampleRate = sMainApp->config.getSampleRate();
    bufferSize = sMainApp->config.getBufferSize();

    channelsNum = 1;
    percentage = 0.0;
}
//------------------------------------
void AnalysisDataSaver::updateFrameRate(){
    //TODO: Remove this reference to config
    frameRate       = sMainApp->config.getFrameRate();
    totalFramesNum  = sMainApp->getTotalFramesNum();
    
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

//------------------------------------
void AnalysisDataSaver::threadedFunction(){
    
    ofSoundBuffer frameSoundBuffer;
    
    while(isThreadRunning()){
      
        if(lock()){
            
            float initTime = ofGetElapsedTimef();
            
            ofxXmlSettings savedSettings;
            
            //cout<<"saving file data"<<endl;
            //FILE-DATA-----------------
            savedSettings.addTag("FILE-INFO");
            savedSettings.pushTag("FILE-INFO");
            savedSettings.addValue("soundfile", soundfilePath);
            savedSettings.addValue("frameRate", frameRate);
            savedSettings.addValue("totalFramesNum", totalFramesNum);
            savedSettings.addValue("durationSeconds", durationSecs);
            savedSettings.addValue("sampleRate", sampleRate);
            savedSettings.addValue("bufferSize", bufferSize);
            savedSettings.addValue("channelsNum", channelsNum);
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
                frameSoundBuffer = sMainApp->timePanel.getSoundBufferMonoForFrame(frameNum, bufferSize);//mono soundbuffer
                
                sMainApp->mainAnalyzer.analyze(frameSoundBuffer);
                
                sMainApp->metersPanel.update();
                
                
                //-:Get Meters values--------------------------------
                vector<std::map<string, float>> metersValues = sMainApp->metersPanel.metersView.getMetersValues();
                vector<std::map<string, vector<float>>> metersVectorValues = sMainApp->metersPanel.metersView.getMetersVectorValues();

                if(metersVectorValues.size() != metersValues.size()){
                    ofLogError("datasSaver threaded function:")<<"metersValues and metersVectorValues sizes not matching.";
                    return;
                }
                
                //--------------
                
                if(metersValues.size() != channelsNum){
                    ofLogError("datasSaver threaded function:")<< "metersPanel channels number incorrect.";
                }
                
                for(int i=0; i<metersValues.size(); i++){
                    
                    //"i" -> channel
                    
                    string channelTag = "CHANNEL-" + ofToString(i);
                    
                    savedSettings.addTag(channelTag);
                    savedSettings.pushTag(channelTag);
                    
                    //->sames order as Osc Indexes (ofxAudioAnalyzerAlgorithms.h)
                    float power         = metersValues[i].at(MTR_NAME_POWER);//0
                    float pitchfreq     = metersValues[i].at(MTR_NAME_PITCH_FREQ);//1
                    float pitchConf     = metersValues[i].at(MTR_NAME_PITCH_CONF);//2
                    float pitchSalience = metersValues[i].at(MTR_NAME_PITCH_SALIENCE);//3
                    float hfc           = metersValues[i].at(MTR_NAME_HFC);//4
                    float centroid      = metersValues[i].at(MTR_NAME_CENTROID);//5
                    float specComp      = metersValues[i].at(MTR_NAME_SPEC_COMP);//6
                    float inharmonicity = metersValues[i].at(MTR_NAME_INHARMONICTY);//7
                    float dissonance    = metersValues[i].at(MTR_NAME_DISSONANCE);//8
                    float rollOff       = metersValues[i].at(MTR_NAME_ROLL_OFF);//9
                    float oddToEven     = metersValues[i].at(MTR_NAME_ODD_TO_EVEN);//10
                    int onset           = metersValues[i].at(MTR_NAME_ONSETS);//11
                    
                    //TODO: set tags as constants/macros
                    //Single Value Algorithms:
                    savedSettings.addValue("POWER", power);
                    savedSettings.addValue("PITCHFREQ", pitchfreq);
                    savedSettings.addValue("PITCHCONF", pitchConf);
                    savedSettings.addValue("SALIENCE", pitchSalience);
                    savedSettings.addValue("HFC", hfc);
                    savedSettings.addValue("CENTROID", centroid);
                    savedSettings.addValue("SPECCOMP", specComp);
                    savedSettings.addValue("INHARM", inharmonicity);
                    savedSettings.addValue("DISSONANCE", dissonance);
                    savedSettings.addValue("ROLLOFF", rollOff);
                    savedSettings.addValue("ODDTOEVEN", oddToEven);
                    savedSettings.addValue("ONSET", onset);
                    
                    if(bSaveVectorValues){
                        
                        
                        //MEL BANDS: ---------------------------
                        savedSettings.addTag("MELBANDS");
                        savedSettings.pushTag("MELBANDS");
                        for (int j=0; j<metersVectorValues[i].at(MTR_NAME_MEL_BANDS).size(); j++){
                            float val = metersVectorValues[i].at(MTR_NAME_MEL_BANDS)[j];
                            savedSettings.addValue(ofToString(j), val);
                        }
                        savedSettings.popTag();//pop from MELBANDS back into CHANNEL-i
                        
                        //MFCC: ---------------------------
                        savedSettings.addTag("MFCC");
                        savedSettings.pushTag("MFCC");
                        for (int j=0; j<metersVectorValues[i].at(MTR_NAME_MFCC).size(); j++){
                            float val = metersVectorValues[i].at(MTR_NAME_MFCC)[j];
                            savedSettings.addValue(ofToString(j), val);
                        }
                        savedSettings.popTag();//pop from MFCC back into CHANNEL-i
                        
                        //HPCP: ---------------------------
                        savedSettings.addTag("HPCP");
                        savedSettings.pushTag("HPCP");
                        for (int j=0; j<metersVectorValues[i].at(MTR_NAME_HPCP).size(); j++){
                            float val = metersVectorValues[i].at(MTR_NAME_HPCP)[j];
                            savedSettings.addValue(ofToString(j), val);
                        }
                        savedSettings.popTag();//pop from HPCP back into CHANNEL-i
                        
                        //TRISTIMULUS: ---------------------------
                        savedSettings.addTag("TRISTIMULUS");
                        savedSettings.pushTag("TRISTIMULUS");
                        for (int j=0; j<metersVectorValues[i].at(MTR_NAME_TRISTIMULUS).size(); j++){
                            float val = metersVectorValues[i].at(MTR_NAME_TRISTIMULUS)[j];
                            savedSettings.addValue(ofToString(j), val);
                        }
                        savedSettings.popTag();//pop from TRISTIMULUS back into CHANNEL-i
                    
                    }
                    
                    //-------------------
                    
                    savedSettings.popTag();//pop from CHANNEL-i back into ANALYZER
                    
                }
                
                savedSettings.popTag();//pop from ANALYZER back into frameTag
                
                //TIMELINE-----------------------------------------
                
                savedSettings.addTag("TIMELINE");
                savedSettings.pushTag("TIMELINE");
                
                sMainApp->timePanel.setCurrentFrame(frameNum);
                std::map<string, float> timelineValues = sMainApp->timePanel.getTracksValues();
                
                for (auto& kv : timelineValues){
                    //cout<<"timeline send osc :: "<<kv.first<<" -- "<<kv.second<<endl;
                    string key = kv.first;
                    float floatValue = kv.second;
                    savedSettings.addValue(key, floatValue);
                    
                }

                savedSettings.popTag();//pop from TIMELINE back into frameTag
                
                //-------------------------------------------------
                
                savedSettings.popTag();//pop from frameTag back into ANALYSIS-DATA
            }
            
            sMainApp->timePanel.setCurrentFrame(0);
        
            
            //save and stop----------------------------------
            string fileName = ANALYSIS_DATA_DIR "/analysisData.xml";
            //savedSettings.saveFile("analysis_data/analysisData.xml");
            
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

