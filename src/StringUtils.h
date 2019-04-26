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

#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"

//#define POWER_STRING "POWER"
//#define PITCH_FREQ_STRING "PITCH-FREQ"
//#define PITCH_CONFIDENCE_STRING "PITCH-CONF"
//#define PITCH_SALIENCE_STRING "PITCH-SALIENCE"
//#define HFC_STRING "HFC"
//#define CENTROID_STRING "CENTROID"
//#define SPEC_COMP_STRING "SPEC-COMP"
//#define INHARMONICITY_STRING "INHARM"
//#define DISSONANCE_STRING "DISONNANCE"
//#define ROLL_OFF_STRING "ROLL-OFF"
//#define ODD_TO_EVEN_STRING "ODD-EVEN"
//#define ONSETS_STRING "ONSETS"
//
//#define SPECTRUM_STRING "SPECTRUM"
//#define MEL_BANDS_STRING "MEL-BANDS"
//#define MFCC_STRING "MFCC"
//#define HPCP_STRING "HPCP"
//#define TRISTIMULUS_STRING "TRISTIMULUS"

namespace utils {
    
    static std::map<string, ofxAAValue> valuesMap = {
        {"RMS", RMS},
        {"POWER", POWER},
        {"ZERO-CROSSING-RATE", ZERO_CROSSING_RATE},
        {"LOUDNESS", LOUDNESS},
//        {"LOUDNESS_VICKERS", LOUDNESS_VICKERS},
        {"SILENCE-RATE-20dB", SILENCE_RATE_20dB},
        {"SILENCE-RATE-30dB", SILENCE_RATE_30dB},
        {"SILENCE-RATE-60dB", SILENCE_RATE_60dB},
        
        {"DYNAMIC_COMPLEXITY", DYNAMIC_COMPLEXITY},
        {"DECREASE", DECREASE},
        {"DISTRIBUTION_SHAPE_KURTOSIS", DISTRIBUTION_SHAPE_KURTOSIS},
        {"DISTRIBUTION_SHAPE_SPREAD", DISTRIBUTION_SHAPE_SPREAD},
        {"DISTRIBUTION_SHAPE_SKEWNESS", DISTRIBUTION_SHAPE_SKEWNESS},
        {"LOG_ATTACK_TIME", LOG_ATTACK_TIME},
        {"STRONG-DECAY", STRONG_DECAY},
        {"FLATNESS-SFX", FLATNESS_SFX},
        {"MAX-TO-TOTAL", MAX_TO_TOTAL},
        {"TC-TO-TOTAL", TC_TO_TOTAL},
        {"DERIVATIVE_SFX_AFTER_MAX", DERIVATIVE_SFX_AFTER_MAX},
        {"DERIVATIVE_SFX_BEFORE_MAX", DERIVATIVE_SFX_BEFORE_MAX},
        
        {"MEL-KURTOSIS", MEL_BANDS_KURTOSIS},
        {"MEL-SPREAD", MEL_BANDS_SPREAD},
        {"MEL-SKEWNESS", MEL_BANDS_SKEWNESS},
        {"MEL-FLATNESS", MEL_BANDS_FLATNESS_DB},
        {"MEL-CREST", MEL_BANDS_CREST},
        {"ERB-KURTOSIS", ERB_BANDS_KURTOSIS},
        {"ERB-SPREAD", ERB_BANDS_SPREAD},
        {"ERB-SKEWNESS", ERB_BANDS_SKEWNESS},
        {"ERB-FLATNESS", ERB_BANDS_FLATNESS_DB},
        {"ERB-CREST", ERB_BANDS_CREST},
        {"BARK-KURTOSIS", BARK_BANDS_KURTOSIS},
        {"BARK-SPREAD", BARK_BANDS_SPREAD},
        {"BARK-SKEWNESS", BARK_BANDS_SKEWNESS},
        {"BARK-FLATNESS", BARK_BANDS_FLATNESS_DB},
        {"BARK-CREST", BARK_BANDS_CREST},
        {"ENERGY-BAND-LOW", ENERGY_BAND_LOW},
        {"ENERGY-BAND-MID-LOW", ENERGY_BAND_MID_LOW},
        {"ENERGY-BAND-MID-HI", ENERGY_BAND_MID_HI},
        {"ENERGY-BAND-HI", ENERGY_BAND_HI},
        
        {"SPEC-KURTOSIS", SPECTRAL_KURTOSIS},
        {"SPEC-SPREAD", SPECTRAL_SPREAD},
        {"SPEC-SKEWNESS", SPECTRAL_SKEWNESS},
        {"SPEC-DECREASE", SPECTRAL_DECREASE},
        {"SPEC-ROLLOFF", SPECTRAL_ROLLOFF},
        {"SPEC-ENERGY", SPECTRAL_ENERGY},
        {"SPEC-ENTROPY", SPECTRAL_ENTROPY},
        {"SPEC-CENTROID", SPECTRAL_CENTROID},
        {"SPEC-COMPLEXITY", SPECTRAL_COMPLEXITY},
        {"SPEC-FLUX", SPECTRAL_FLUX},
        {"DISSONANCE", DISSONANCE},
        {"HFC", HFC},
        {"PITCH-SALIENCE", PITCH_SALIENCE},
        {"INHARMONICITY", INHARMONICITY},
        {"ODD-EVEN", ODD_TO_EVEN},
        {"STRONG-PEAK", STRONG_PEAK},
        {"HPCP-CREST", HPCP_CREST},
        {"HPCP-ENTROPY", HPCP_ENTROPY},
        {"PITCH-FREQUENCY", PITCH_YIN_FREQUENCY},
        {"PITCH-CONFIDENCE", PITCH_YIN_CONFIDENCE},
        {"ONSETS", ONSETS},
        {"NONE", NONE}
    };
    
    static std::map<string, ofxAABinsValue> binValuesMap = {
        {"SPECTRUM", SPECTRUM},
        {"MEL-BANDS", MFCC_MEL_BANDS},
        {"GFCC-ERB-BANDS", GFCC_ERB_BANDS},
        {"BARK-BANDS", BARK_BANDS},
        {"TRISTIMULUS", TRISTIMULUS},
        {"HPCP", HPCP},
        {"PITCH_MELODIA_FREQUENCIES", PITCH_MELODIA_FREQUENCIES},
        {"PITCH_MELODIA_CONFIDENCES", PITCH_MELODIA_CONFIDENCES},
        {"PREDOMINANT_PITCH_MELODIA_FREQUENCIES", PREDOMINANT_PITCH_MELODIA_FREQUENCIES},
        {"PREDOMINANT_PITCH_MELODIA_CONFIDENCES", PREDOMINANT_PITCH_MELODIA_CONFIDENCES},
        {"NONE_BINS", NONE_BINS}
    };
    
    string valueTypeToString(ofxAAValue value);
    string binsValueTypeToString(ofxAABinsValue value);
    
    ofxAAValue stringToValueType(string stringType);
    ofxAABinsValue stringToBinsValueType(string stringType);
    
}
