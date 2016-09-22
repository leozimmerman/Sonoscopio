
#include "AnalysisDataSaver.h"

#include "ofApp.h"

ofApp* sMainApp;
analysisMode currentAnalysisMode;

//------------------------------------
void AnalysisDataSaver::setup(ofBaseApp* appPtr){
    
    sMainApp = dynamic_cast<ofApp*>(appPtr);
    
    frameRate = sMainApp->getFrameRate();
    totalFramesNum = sMainApp->getTotalFramesNum();
    
    sampleRate = sMainApp->getSampleRate();
    bufferSize = sMainApp->getBufferSize();
    
    currentAnalysisMode = sMainApp->getAnalysisMode();
    
    if(currentAnalysisMode==SPLIT){
        channelsNum = sMainApp->getChannelsNum();
    }
    else{
        channelsNum = 1;
    }
    
    percentage = 0.0;
    
}
//------------------------------------
void AnalysisDataSaver::reset(){
    
    if(sMainApp==NULL){
        ofLogError("AnalysisDataSaver: ") <<"mainApp ptr not assigned. Need to call setup function first.";
        return;
    }
    
    frameRate = sMainApp->getFrameRate();
    totalFramesNum = sMainApp->getTotalFramesNum();
    
    sampleRate = sMainApp->getSampleRate();
    bufferSize = sMainApp->getBufferSize();

    
    currentAnalysisMode = sMainApp->getAnalysisMode();
    
    if(currentAnalysisMode==SPLIT){
        channelsNum = sMainApp->getChannelsNum();
    }
    else{
        channelsNum = 1;
    }
    percentage = 0.0;
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
            savedSettings.addValue("frameRate", frameRate);
            savedSettings.addValue("totalFramesNum", totalFramesNum);
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
           
                if(currentAnalysisMode==SPLIT){
                    frameSoundBuffer = sMainApp->timePanel.audioTrack->getSoundBufferForFrame(frameNum, bufferSize);//multichannel soundbuffer
                }else if(currentAnalysisMode==MONO){
                    frameSoundBuffer = sMainApp->timePanel.audioTrack->getSoundBufferMonoForFrame(frameNum, bufferSize);//mono soundbuffer
                }
                
                sMainApp->mainAnalyzer.analyze(frameSoundBuffer);
                sMainApp->metersPanel.update();
                //---------------------------------
                vector<std::map<string, float>> metersValues = sMainApp->metersPanel.getMetersValues();
                
                if(metersValues.size()!= channelsNum){
                    ofLogError("datasSaver threaded function:")<< "metersPanel channels number incorrect.";
                }
                

                for(int i=0; i<metersValues.size(); i++){
                    
                    string channelTag = "CHANNEL-" + ofToString(i);
                    
                    savedSettings.addTag(channelTag);
                    savedSettings.pushTag(channelTag);
                    
                    float power         = metersValues[i].at(MTR_NAME_POWER);
                    float pitchfreq     = metersValues[i].at(MTR_NAME_PITCH_FREQ);
                    float pitchConf     = metersValues[i].at(MTR_NAME_PITCH_CONF);
                    float pitchSalience = metersValues[i].at(MTR_NAME_PITCH_SALIENCE);
                    float hfc           = metersValues[i].at(MTR_NAME_HFC);
                    float centroid      = metersValues[i].at(MTR_NAME_CENTROID);
                    float specComp      = metersValues[i].at(MTR_NAME_SPEC_COMP);
                    float inharmonicity = metersValues[i].at(MTR_NAME_INHARMONICTY);
                    int onset           = metersValues[i].at(MTR_NAME_ONSETS);
                    
                    //TODO: set tags as constants/macros
                    savedSettings.addValue("POWER", power);
                    savedSettings.addValue("PITCHFREQ", pitchfreq);
                    savedSettings.addValue("PITCHCONF", pitchConf);
                    savedSettings.addValue("SALIENCE", pitchSalience);
                    savedSettings.addValue("HFC", hfc);
                    savedSettings.addValue("CENTROID", centroid);
                    savedSettings.addValue("SPECCOMP", specComp);
                    savedSettings.addValue("INHARM", inharmonicity);
                    savedSettings.addValue("ONSET", onset);
                
                    savedSettings.popTag();//pop from CHANNEL-i back into ANALYZER
                    
                }
                
                savedSettings.popTag();//pop from ANALYZER back into frameTag
                
                //TIMELINE-----------------------------------------
                
                savedSettings.addTag("TIMELINE");
                savedSettings.pushTag("TIMELINE");
                
                sMainApp->timePanel.timeline.setCurrentFrame(frameNum);
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
            
            sMainApp->timePanel.timeline.setCurrentFrame(0);
        
            
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

