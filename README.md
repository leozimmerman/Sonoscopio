#SONOSCOPIO
In development.

##Projects & Settings

* If none project is opened, it will save and load settings to
 - bin/data/main_settings/
 - bin/data/meters_settings/
 - bin/data/timeline_settings/
* "Open Project" dropdown button list projects in bin/data/projects
* If a Project is open, "Load Settings" and "Save Settings" will load/save to the project's settings files.
* A project folder must have:
 - audiofile.wav or audiofile.mp3
 - main_settings/
 - meters_settings/
 - timeline_settings/ 
* Creating a New Project:
 - Open app with no project loaded
 - Open audio file, set settings and save them.
 - Create a folder in bin/data/projects with the projects name.
 - Copy the file used and rename it "audiofile".
 - Copy the settings files from bin/data to bin/data/projects/PROJECT_NAME/  

## OSC
 - Por audio channel (ch0, ch1, ch2, etc):
     + address "/ch0" : all Single Values in sames order as Osc Indexes (ofxAudioAnalyzerAlgorithms.h)
     + address "/ch0mel": Mel Bands - size=24
     + address "/ch0mfcc": MFCC - size=13
     + address "/ch0hpcp": HPCP - size=12
     + address "/ch0tris": Tristimulus - size=3

 - Por Timeline Track:
     + address "/TL-(trackname)": El value del track 
     
## Keys Commands

* 't' : time measurement on/off
* 'm' : add marker
* 'w' : rewind
* 'e' : expands focused track
* 'd' : enables/disables focused track
* 'a' : adjust tracks height shorcut
