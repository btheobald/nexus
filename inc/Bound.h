//
// Created by byron on 30/03/17.
//

#ifndef NEXUS_BOUNDS_H
#define NEXUS_BOUNDS_H


#include "Vec2f.h"
#include "Body.h"

class Bound {
private:
    float centerX;
    float centerY;
    float half;

public:
    Bound();
    Bound(float x, float y, float half);

    int getQuadrant(float x, float y);
    Bound* getNewBound(int q);
    inline float getWidth() { return half*2; };
    inline float getCenter(int xy) { if(xy) return centerY; else return centerX; };
};


#endif //NEXUS_BOUNDS_H
