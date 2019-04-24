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
        {"ZERO_CROSSING_RATE", ZERO_CROSSING_RATE},
        {"LOUDNESS", LOUDNESS},
        {"LOUDNESS_VICKERS", LOUDNESS_VICKERS},
        {"SILENCE_RATE_20dB", SILENCE_RATE_20dB},
        {"SILENCE_RATE_30dB", SILENCE_RATE_30dB},
        {"SILENCE_RATE_60dB", SILENCE_RATE_60dB},
        
        {"DYNAMIC_COMPLEXITY", DYNAMIC_COMPLEXITY},
        {"DECREASE", DECREASE},
        {"DISTRIBUTION_SHAPE_KURTOSIS", DISTRIBUTION_SHAPE_KURTOSIS},
        {"DISTRIBUTION_SHAPE_SPREAD", DISTRIBUTION_SHAPE_SPREAD},
        {"DISTRIBUTION_SHAPE_SKEWNESS", DISTRIBUTION_SHAPE_SKEWNESS},
        {"LOG_ATTACK_TIME", LOG_ATTACK_TIME},
        {"STRONG_DECAY", STRONG_DECAY},
        {"FLATNESS_SFX", FLATNESS_SFX},
        {"MAX_TO_TOTAL", MAX_TO_TOTAL},
        {"TC_TO_TOTAL", TC_TO_TOTAL},
        {"DERIVATIVE_SFX_AFTER_MAX", DERIVATIVE_SFX_AFTER_MAX},
        {"DERIVATIVE_SFX_BEFORE_MAX", DERIVATIVE_SFX_BEFORE_MAX},
        
        {"MEL_BANDS_KURTOSIS", MEL_BANDS_KURTOSIS},
        {"MEL_BANDS_SPREAD", MEL_BANDS_SPREAD},
        {"MEL_BANDS_SKEWNESS", MEL_BANDS_SKEWNESS},
        {"MEL_BANDS_FLATNESS_DB", MEL_BANDS_FLATNESS_DB},
        {"MEL_BANDS_CREST", MEL_BANDS_CREST},
        {"ERB_BANDS_KURTOSIS", ERB_BANDS_KURTOSIS},
        {"ERB_BANDS_SPREAD", ERB_BANDS_SPREAD},
        {"ERB_BANDS_SKEWNESS", ERB_BANDS_SKEWNESS},
        {"ERB_BANDS_FLATNESS_DB", ERB_BANDS_FLATNESS_DB},
        {"ERB_BANDS_CREST", ERB_BANDS_CREST},
        {"BARK_BANDS_KURTOSIS", BARK_BANDS_KURTOSIS},
        {"BARK_BANDS_SPREAD", BARK_BANDS_SPREAD},
        {"BARK_BANDS_SKEWNESS", BARK_BANDS_SKEWNESS},
        {"BARK_BANDS_FLATNESS_DB", BARK_BANDS_FLATNESS_DB},
        {"BARK_BANDS_CREST", BARK_BANDS_CREST},
        {"ENERGY_BAND_LOW", ENERGY_BAND_LOW},
        {"ENERGY_BAND_MID_LOW", ENERGY_BAND_MID_LOW},
        {"ENERGY_BAND_MID_HI", ENERGY_BAND_MID_HI},
        {"ENERGY_BAND_HI", ENERGY_BAND_HI},
        
        {"SPECTRAL_KURTOSIS", SPECTRAL_KURTOSIS},
        {"SPECTRAL_SPREAD", SPECTRAL_SPREAD},
        {"SPECTRAL_SKEWNESS", SPECTRAL_SKEWNESS},
        {"SPECTRAL_DECREASE", SPECTRAL_DECREASE},
        {"SPEC-ROLLOFF", SPECTRAL_ROLLOFF},
        {"SPECTRAL_ENERGY", SPECTRAL_ENERGY},
        {"SPECTRAL_ENTROPY", SPECTRAL_ENTROPY},
        {"SPECTRAL_CENTROID", SPECTRAL_CENTROID},
        {"SPECTRAL_COMPLEXITY", SPECTRAL_COMPLEXITY},
        {"SPECTRAL_FLUX", SPECTRAL_FLUX},
        {"DISSONANCE", DISSONANCE},
        {"HFC", HFC},
        {"PITCH_SALIENCE", PITCH_SALIENCE},
        {"INHARMONICITY", INHARMONICITY},
        {"ODD-EVEN", ODD_TO_EVEN},
        {"HPCP_CREST", HPCP_CREST},
        {"HPCP_ENTROPY", HPCP_ENTROPY},
        {"PITCH_YIN_FREQUENCY", PITCH_YIN_FREQUENCY},
        {"PITCH_YIN_CONFIDENCE", PITCH_YIN_CONFIDENCE},
        {"ONSETS", ONSETS},
        {"NONE", NONE}
    };
    
    static std::map<string, ofxAABinsValue> binValuesMap = {
        {"SPECTRUM", SPECTRUM}, {"MFCC_MEL_BANDS", MFCC_MEL_BANDS},
        {"GFCC_ERB_BANDS", GFCC_ERB_BANDS},
        {"BARK_BANDS", BARK_BANDS},
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
