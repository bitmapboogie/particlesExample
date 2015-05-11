//
//  Styling.h
//
//
//  Created by Fabian Moron Zirfas on 03.05.15.
//
// usage
//     ofFill();
// ofBackground(style.getBackgroundColor());
// ofSetColor(style.getForgroundColor());
// ofRect(0,0,ofGetWidth(),100);
// ofNoFill();
// ofImage img = style.getLogo();
// ofSetColor(255,255,255,200);
// float w = img.getWidth();
// float h = img.getHeight();
// img.resize(w/5, h/5);
// ofEnableAlphaBlending();
// img.draw(25,ofGetHeight() - (img.getHeight() + 25));
// ofDisableAlphaBlending();

#ifndef __Michelin__Styling__
#define __Michelin__Styling__
#include "ofMain.h"

class Styling {
    
public:
    Styling();
    ofColor getForgroundColor();
    ofColor getBackgroundColor();
    ofColor getGradientColorOne();
    ofColor getGradientColorTwo();
    //    void setForgroundColor();
    //    void setBackgroundColor();
//    ofImage getLogo();
private:
    ofColor fg;
    ofColor bg;
    ofColor grad01;
    ofColor grad02;
//    ofImage logo;
    
};

#endif /* defined(__Michelin__Styling__) */

