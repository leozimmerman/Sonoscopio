//
//  AddTrackModal.hpp
//  Sonoscopio
//
//  Created by Leonardo Zimmerman on 11/12/2018.
//

#include "ofMain.h"
#include "ofxModal.h"
#include "Settings.h"
#include "TimelineView.h"

#define TRACK_TYPE_LABEL "TRACK TYPE"
#define TRACK_NAME_LABEL "TRACK NAME"

class AddTrackModal : public ofxModalWindow {
public:
    
    AddTrackModal(TimelineView* timelineView_ptr);
    
    void display(int height);
    void applyConfiguration();
    bool getFocused();
    
    
private:
    
    void onApplyButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);

    
    ofxDatGuiTextInput* gTrackNameTextInput;
    
    TimelineView* _timelineViewPtr;
    
    string selectedTrackName;
    string selectedTrackType;
    
    const vector<string> options = {CURVES_STRING, BANGS_STRING, SWITCHES_STRING, NOTES_STRING};
};
