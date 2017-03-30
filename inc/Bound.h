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
    Bound(Vec2f center, float half);

    int getQuadrant(Vec2f p);
    Bound* getNewBound(int q);
};


#endif //NEXUS_BOUNDS_H
