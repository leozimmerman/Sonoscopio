#!/bin/bash

#Dependencies of Sonoscopio

if [ -z $1 ]; then
PREFIX="git clone https://github.com/"
else
PREFIX="git clone git@github.com:"
fi

cd ../../../addons

#ofxAudioAnalyzer------------------------
${PREFIX}leozimmerman/ofxAudioAnalyzer.git

#ofxAudioDecoder------------------------
${PREFIX}leozimmerman/ofxAudioDecoder.git

#ofxDatGui------------------------
${PREFIX}leozimmerman/ofxGpuParticles.git
cd ofxDatGui
git checkout sonoscopio
cd ../

#ofxMSATimer------------------------
${PREFIX}obviousjim/ofxMSATimer.git
cd ofxMSATimer
git checkout 14e1ff8b0a358151ba13efadac96cf1c50266518
cd ../

#ofxRange------------------------
${PREFIX}Flightphase/ofxRange.git
cd ofxRange
git checkout 8e232a33a0db5007c425c3d1ebc7f57ffb2232ac
cd ../

#ofxTextInputField------------------------
${PREFIX}elliotwoods/ofxTextInputField.git
cd ofxTextInputField
git checkout e265bee9547cc1790395f9a1b83289e4e11a7624
cd ../

#ofxTimeMeasurements------------------------
${PREFIX}armadillu/ofxTimeMeasurements.git
cd ofxTimeMeasurements
git checkout 47fd27f73850dcaf27ffb5dcb339cbbc04e1af91
cd ../

#ofxTimecode------------------------
${PREFIX}YCAMInterlab/ofxTimecode.git
cd ofxTimecode
git checkout d8b6b97e1334453753b369ace86e0ce7113946b3
cd ../

#ofxTimeline------------------------
${PREFIX}leozimmerman/ofxTimeline.git
cd ofxTimeline
git checkout sonoscopio
cd ../

#ofxTween------------------------
${PREFIX}obviousjim/ofxTween.git
cd ofxTween
git checkout 120757d613b497a7045ee64c1e548abf31ace38e
cd ../




