//
//  ofxAAUtils.cpp
//  Sonoscopio
//
//  Created by Leo on 11/09/2018.
//

#include "ofxAAUtils.h"

namespace ofxaa{
    string algorithmTypeToString(ofxAAAlgorithmType algorithmType) {
        switch (algorithmType) {
            case RMS: return "RMS";
            case PITCH_FREQ: return "PITCH_FREQ";
            case PITCH_CONFIDENCE: return "PITCH_CONFIDENCE";
            case PITCH_SALIENCE: return "PITCH_SALIENCE";
            case HFC: return "HFC";
            case CENTROID: return "CENTROID";
            case SPECTRAL_COMPLEXITY: return "SPECTRAL_COMPLEXITY";
            case INHARMONICITY: return "INHARMONICITY";
            case DISSONANCE: return "DISSONANCE";
            case ROLL_OFF: return "ROLL_OFF";
            case ODD_TO_EVEN: return "ODD_TO_EVEN";
            case ONSETS: return "ONSETS";
            case SPECTRUM: return "SPECTRUM";
            case MEL_BANDS: return "MEL_BANDS";
            case MFCC: return "MFCC";
            case HPCP: return "HPCP";
            case TRISTIMULUS: return "TRISTIMULUS";
            default: return "";
        }
    }
    
}
