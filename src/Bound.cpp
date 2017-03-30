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

Bound::Bound(Vec2f center, float _half) {
    centerX = center.get(0);
    centerY = center.get(1);
    half = _half;
}

int Bound::getQuadrant(Vec2f p) {
    if((p.get(0) <= centerX) & (p.get(1) >  centerY))
        return 0;
    if((p.get(0) >  centerX) & (p.get(1) >  centerY))
        return 1;
    if((p.get(0) <= centerX) & (p.get(1) <= centerY))
        return 2;
    if((p.get(0) >  centerX) & (p.get(1) <= centerY))
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
