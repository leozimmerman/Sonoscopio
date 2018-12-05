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
            case PITCH_CONFIDENCE: return "PITCH_CONF";
            case PITCH_SALIENCE: return "PITCH_SLNCE";
            case HFC: return "HFC";
            case CENTROID: return "CENTROID";
            case SPECTRAL_COMPLEXITY: return "SPEC_COMP";
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
    
    ofxAAAlgorithmType stringToAlgorithmType(string stringType){
        if (stringType == algorithmTypeToString(RMS)){
            return RMS;
        } else if (stringType == algorithmTypeToString(PITCH_FREQ)){
            return PITCH_FREQ;
        } else if (stringType == algorithmTypeToString(PITCH_CONFIDENCE)){
            return PITCH_CONFIDENCE;
        } else if (stringType == algorithmTypeToString(PITCH_SALIENCE)){
            return PITCH_SALIENCE;
        } else if (stringType == algorithmTypeToString(HFC)){
            return HFC;
        } else if (stringType == algorithmTypeToString(CENTROID)){
            return CENTROID;
        } else if (stringType == algorithmTypeToString(SPECTRAL_COMPLEXITY)){
            return SPECTRAL_COMPLEXITY;
        } else if (stringType == algorithmTypeToString(INHARMONICITY)){
            return INHARMONICITY;
        } else if (stringType == algorithmTypeToString(DISSONANCE)){
            return DISSONANCE;
        } else if (stringType == algorithmTypeToString(ROLL_OFF)){
            return ROLL_OFF;
        } else if (stringType == algorithmTypeToString(ODD_TO_EVEN)){
            return ODD_TO_EVEN;
        } else if (stringType == algorithmTypeToString(ONSETS)){
            return ONSETS;
        } else if (stringType == algorithmTypeToString(SPECTRUM)){
            return SPECTRUM;
        }else if (stringType == algorithmTypeToString(MEL_BANDS)){
            return MEL_BANDS;
        }else if (stringType == algorithmTypeToString(MFCC)){
            return MFCC;
        }else if (stringType == algorithmTypeToString(HPCP)){
            return HPCP;
        }else if (stringType == algorithmTypeToString(TRISTIMULUS)){
            return TRISTIMULUS;
        }
    }
    
}
