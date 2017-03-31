//
// Created by byron on 30/03/17.
//

#include "Bound.h"

Bound::Bound() {

}

Bound::Bound(float x, float y, float _half) {
    centerX = x;
    centerY = y;
    half = _half;
}

int Bound::getQuadrant(float x, float y) {
    if((x <= centerX) & (y >  centerY))
        return 0;
    if((x >  centerX) & (y >  centerY))
        return 1;
    if((x <= centerX) & (y <= centerY))
        return 2;
    if((x >  centerX) & (y <= centerY))
        return 3;
    return -1;
}

Bound* Bound::getNewBound(int q) {
    float cX;
    float cY;

    // Set new half distance
    float hd = half/2;

    // Set new center point
    if(q == 0) {
        cX = centerX - hd;
        cY = centerY + hd;
    }
    if(q == 1) {
        cX = centerX + hd;
        cY = centerY + hd;
    }
    if(q == 2) {
        cX = centerX - hd;
        cY = centerY - hd;
    }
    if(q == 3) {
        cX = centerX + hd;
        cY = centerY - hd;
    }

    // Return new calculated bounds
    return new Bound(cX, cY, hd);
}
