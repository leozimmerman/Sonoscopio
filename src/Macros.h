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

//#define METERS_SETTINGS_DIR "meters_settings"
//#define MAIN_SETTINGS_DIR "main_settings"
#define TIMELINE_SETTINGS_DIR "timeline_settings"

#define ANALYSIS_DATA_DIR "analysis_data"

#define INIT_FPS 60
//Prime numbers to avoid simultaneous draws:
#define MAIN_PANEL_FPS 60
#define TL_PANEL_FPS 60
#define MT_PANEL_FPS 60
#define GUI_STATIC_FPS 60

#define INIT_AUDIO_FILE "audio_files/baby-weird.wav"
//#define INIT_AUDIO_FILE ""
#define INIT_BUFFER_SIZE 512
#define INIT_PROJECT_DIR ""
#define INIT_OSC_HOST "localhost"
#define INIT_OSC_PORT 12345

#define GUI_COMP_HEIGHT 26

//-:NAMES
#define MTR_NAME_POWER "POWER"
#define MTR_NAME_PITCH_FREQ "FREQ"
#define MTR_NAME_PITCH_CONF "CONFID"
#define MTR_NAME_PITCH_SALIENCE "SALIENCE"
#define MTR_NAME_HFC "HFC"
#define MTR_NAME_CENTROID "CENTROID"
#define MTR_NAME_SPEC_COMP "SPEC-COMP"
#define MTR_NAME_INHARMONICTY "INHARM"
#define MTR_NAME_DISSONANCE "DISONNANCE"
#define MTR_NAME_ROLL_OFF "ROLL-OFF"
#define MTR_NAME_ODD_TO_EVEN "ODD-EVEN"
#define MTR_NAME_SPECTRUM "SPECTRUM"
#define MTR_NAME_MEL_BANDS "MEL-BANDS"
#define MTR_NAME_MFCC "MFCC"
#define MTR_NAME_HPCP "HPCP"
#define MTR_NAME_TRISTIMULUS "TRISTIMULUS"
#define MTR_NAME_ONSETS "ONSETS"
#define MTR_SMOOTHING "SMTH"
#define MTR_ON_OFF "ON"
#define MTR_PEAK "PEAK"
