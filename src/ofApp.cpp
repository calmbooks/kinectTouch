#include "ofApp.h"

void ofApp::setup() {
    
    ofSetFrameRate(30);
    
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    // kinect.setCameraTiltAngle(0);
    
    grayImage.allocate(kinect.width, kinect.height);
    
    gui.setup();
    gui.add(THRESHOLD_FLOOR_BRIGHTNESS.setup("Floor brightness", 130, 100, 240));
    gui.add(CALIBRATION_mm_min.setup("mm min", 400, 300, 1600));
    gui.add(CALIBRATION_mm_max.setup("mm max", 1000, 300, 1600));
    gui.add(CALIBRATION_bri_min.setup("bri min", 161, 100, 200));
    gui.add(CALIBRATION_bri_max.setup("bri max", 204, 155, 255));
    
    targetRect.setup(gui);
    
    validPixelNum = int(targetRect.width * targetRect.height * 0.4);
    
    gui.loadFromFile("settings.xml");
    bDebugMode = true;
}

void ofApp::update() {
    
    kinect.update();
    if( !kinect.isFrameNew() ) return;
    
    updateGrayImage();
    
    ofPixels pixel = kinect.getDepthPixelsRef();
    contourFinder.findContours(grayImage, 5, 10000, 10, false);
    
    touchArray.clear();
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        // if( contourFinder.blobs[i].nPts > 10) {
            touchArray.push_back(getTouchPoint(contourFinder.blobs[i].pts, pixel));
        // }
    }
    
    bTouch = ( touchArray.size() > 0 ) ? true : false;
    
}

void ofApp::updateGrayImage() {
    unsigned char * grayPixels = kinect.getDepthPixels();
    int numPixels = kinect.width * kinect.height;
    
    for(int i = 0; i < numPixels; i++) {
        if( grayPixels[i] < THRESHOLD_FLOOR_BRIGHTNESS ) {
            grayPixels[i] = 0;
        }
        int x = i % kinect.width;
        int y = i / kinect.width;
        if( targetRect.x < x && x < ( targetRect.x + targetRect.width ) ) {
            if( targetRect.y < y && y < ( targetRect.y + targetRect.height ) ) {
            }
            else {
                grayPixels[i] = 0;
            }
        }
        else {
            grayPixels[i] = 0;
        }
    }
    grayImage.setFromPixels(grayPixels, kinect.width, kinect.height);
}

ofPoint ofApp::getTouchPoint( vector<ofPoint> pts, ofPixels pixel ) {
    ofPoint returnPt;
    
    float mxbri = 0;
    
    for( int i = 0, imax = pts.size(); i < imax; ++i ) {
        ofColor color = pixel.getColor(pts[i].x, pts[i].y);
        float bri = color.getBrightness();
        mxbri = max(mxbri, bri);

    }
    
    float tucX = getTouchX(pixel, mxbri);
    
    float x_ratio = 1 - (tucX - 120) / (520 - 120);
    float y_ratio = (mxbri - 177) / (270 - 177);
    
    if( x_ratio >= 0.5 ) {
        x_ratio = (x_ratio - 0.5) * (1 - y_ratio + 0.15) + 0.5;
    }
    else if( x_ratio < 0.5 ) {
        x_ratio = 0.5 - ((0.5 - x_ratio) * (1 - y_ratio + 0.15));
    }
    
    returnPt.x = x_ratio * (946 - 276) + 276;
    returnPt.y = y_ratio * 780;
    
    return returnPt;
}

float ofApp::getTouchX( ofPixels pixel, float max_bri ) {
    
    float max_x = 0;
    float min_x = 10000;
    
    for ( int x = targetRect.x, xmax = targetRect.x + targetRect.width; x < xmax; ++x ) {
        for ( int y = targetRect.y, ymax = targetRect.y + targetRect.height; y < ymax; ++y ) {
            ofColor color = pixel.getColor(x,y);
            float bri = color.getBrightness();
            if( bri == max_bri ) {
                max_x = max(max_x, float(x));
                min_x = min(min_x, float(x));
            }
        }
    }
    
    return min_x + (( max_x - min_x) * 0.5);
}

void ofApp::draw() {
    
    ofBackground(0, 0, 0);
    
    if( bDebugMode ){
        ofSetColor(255, 255, 255);
        // kinect.drawDepth(0, 0);
        grayImage.draw(0, 0);
        ofSetColor(255, 0, 0);
        // ofDrawBitmapString(ofToString(max_bri), 550, 20);
        // ofDrawBitmapString(ofToString(touchX), 550, 50);
        // ofRect(0,480-(resultHeight*0.1),30,(resultHeight*0.1));
        targetRect.draw();
        gui.draw();
        
        for (int i = 0; i < contourFinder.nBlobs; i++){
            contourFinder.blobs[i].draw(0,0);
        }
    }
    
    if( bTouch ) {
        ofFill();
        ofSetHexColor(0xFF0000);
        // float x = (( 1 - (touchX - 120) / (520 - 120)) * (896 - 326)) + 322;
        // float y = (max_bri - 177) / (270 - 177) * 780;
        //322 896
        for( int i = 0, imax = touchArray.size(); i < imax; ++i ) {
            ofCircle(touchArray[i].x, touchArray[i].y, 25);
        }
        ofSetHexColor(0xffffff);
    }
    
    // contourFinder.blobs[1].pts[1].x;
}

void ofApp::exit() {
    // kinect.setCameraTiltAngle(0);
    kinect.close();
}

void ofApp::keyPressed( int key ){
    if( key == 'd' ){
        bDebugMode = !bDebugMode;
    }
}


