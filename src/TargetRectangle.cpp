
#include "TargetRectangle.h"

void TargetRectangle::setup( ofxPanel& gui ) {
    
    gui.add(x.setup("Target Rect x", 275, 0, 640));
    gui.add(y.setup("Target Rect y", 222, 0, 320));
    gui.add(width.setup("Target Rect w", 70, 10, 640));
    gui.add(height.setup("Target Rect h", 40, 10, 500));
}

void TargetRectangle::draw() {
    ofSetColor(0,0,255);
    ofNoFill();
    ofSetLineWidth(2);
    ofRect(x, y, width, height);
}
