//
//  ofxAAUtils.hpp
//  Sonoscopio
//
//  Created by Leo on 11/09/2018.
//
#pragma once

#include "ofMain.h"

#include "ofxAudioAnalyzer.h"

namespace ofxaa {
    string algorithmTypeToString(ofxAAAlgorithmType algorithmType);
    ofxAAAlgorithmType stringToAlgorithmType(string stringType);
    
    static const vector<ofxAAAlgorithmType> allAvailableAlgorithmTypes = {RMS, PITCH_FREQ, PITCH_CONFIDENCE, PITCH_SALIENCE, HFC, CENTROID, SPECTRAL_COMPLEXITY, INHARMONICITY, DISSONANCE, ROLL_OFF, ODD_TO_EVEN, ONSETS, SPECTRUM, MEL_BANDS, MFCC, HPCP, TRISTIMULUS};
}
