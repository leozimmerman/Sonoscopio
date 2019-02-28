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

#define SONOSCOPIO_VERSION "0.1.0"

#define INIT_FPS 60
#define TL_DEFAULT_INIT_BPM 120.0

//Prime numbers to avoid simultaneous draws:
#define MAIN_PANEL_FPS 60
#define TL_PANEL_FPS 60
#define MT_PANEL_FPS 60
#define GUI_STATIC_FPS 60

#define INIT_SAMPLE_RATE 44100
#define INIT_BUFFER_SIZE 512
#define INIT_PROJECT_DIR ""
#define INIT_OSC_HOST "localhost"
#define INIT_OSC_PORT 12345
#define INIT_OSC_SEND_VECTOR false

#define GUI_COMP_HEIGHT 26

//TODO: Fix this:
#define KEYBOARD_SHORTCUTS_MSG "'cmd 1,2,3: view modes | 'cmd + t': time measurement on/off  |  'cmd + m': add marker  |  'w': rewind  |  'e': expands focused track  |  'd': enables/disables focused track  |  'a': adjust tracks height shortcut"
