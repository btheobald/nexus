//
// Created by byron on 30/03/17.
//

#include "Body.h"

Body::Body() {
    mass = 1;
    radius = 1;
    pos = Vec2f();
    vel = Vec2f();
    acc = Vec2f();
}

Body::Body(float _m, float _x, float _y, float _vx, float _vy) {
    mass = _m;
    radius = 1;
    pos = Vec2f(_x, _y);
    vel = Vec2f(_vx, _vy);
    acc = Vec2f();
}

void Body::simUpdatePos(float dt) {
    pos += vel*dt;
}

void Body::simUpdateVel(float dt) {
    pos += acc*dt;
}