#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class TargetRectangle : public ofBaseApp {
    
public:
    void setup( ofxPanel& gui );
    void draw();
    
    ofxIntSlider x;
    ofxIntSlider y;
    ofxIntSlider width;
    ofxIntSlider height;
};