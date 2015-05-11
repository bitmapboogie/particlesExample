//
//  Styling.cpp
//  Michelin
//
//  Created by Fabian Moron Zirfas on 03.05.15.
//
//

#include "Styling.h"

Styling::Styling(){
    fg = ofColor::fromHex(0xfdf733);// yellow #fdf733
    bg = ofColor::fromHex(0x0b2b88); //std blue #0b2b88
    grad01 = ofColor::fromHex(0x1363ed); // lighter blue 1363ed
    grad02 = bg;
  //  logo.loadImage("2_TER-HELL-Plastic-GmbH.gif");
}

ofColor Styling::getBackgroundColor(){
    return bg;
}

ofColor Styling::getForgroundColor(){
    return fg;
}

ofColor Styling::getGradientColorOne(){
    return grad01;
}

ofColor Styling::getGradientColorTwo(){
    return grad02;
}
//ofImage Styling::getLogo(){
 //   return logo;
//}