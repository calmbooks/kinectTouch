#pragma once

#define DEBUG_MODE

#include "ofMain.h"
#include "ofxGui.h"

#include "TargetRectangle.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void updateGrayImage();
    void draw();
    void exit();
    void keyPressed( int key );
    
    ofPoint getTouchPoint( vector<ofPoint> pts, ofPixels pixel );
    float getTouchX( ofPixels pixel, float bri );
    
    ofxKinect kinect;
    ofxCvGrayscaleImage grayImage;
    
    TargetRectangle targetRect;
    
    
    bool bDebugMode;
    ofxPanel gui;
    
    bool bTouch;
    
    ofxIntSlider THRESHOLD_FLOOR_BRIGHTNESS;
    ofxIntSlider CALIBRATION_mm_min;
    ofxIntSlider CALIBRATION_mm_max;
    ofxIntSlider CALIBRATION_bri_min;
    ofxIntSlider CALIBRATION_bri_max;
    
    int validPixelNum;
    
    vector<ofPoint> touchArray;
    
    ofxCvContourFinder 	contourFinder;
};
