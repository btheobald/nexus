//
// Created by byron on 30/03/17.
//

#include <cstdlib>
#include "Body.h"

Body::Body() {
    mass = 1;
    radius = 1;

    pos[0] = 0.0;
    pos[1] = 0.0;

    vel[0] = 0.0;
    vel[1] = 0.0;

    acc[0] = 0.0;
    acc[1] = 0.0;
}

Body::Body(float _m, float _x, float _y, float _vx, float _vy) {
    mass = _m;
    radius = 1;

    pos[0] = _x;
    pos[1] = _y;

    vel[0] = _vx;
    vel[1] = _vy;

    acc[0] = 0.0;
    acc[1] = 0.0;
}

Body::~Body() {

}

void Body::simUpdatePos(float dt) {
    pos[0] += vel[0]*dt;
    pos[1] += vel[1]*dt;
}

void Body::simUpdateVel(float dt) {
    vel[0] += acc[0]*dt;
    vel[1] += acc[1]*dt;
}