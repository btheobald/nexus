//
// Created by byron on 30/03/17.
//

#include "Vec2f.h"

Vec2f::Vec2f(float _x, float _y) {
    x = _x;
    y = _y;
}

Vec2f::Vec2f(Vec2f *c) {
    *this = *c;
}

Vec2f::~Vec2f() {

}

float Vec2f::get(int xy) {
    if(xy) return y;
    else return x;
}

void Vec2f::set(float _x, float _y) {
    x = _x;
    y = _y;
}

void Vec2f::operator+=(const Vec2f &b) {
    this->x = this->x + b.x;
    this->y = this->y + b.y;
}

Vec2f Vec2f::operator+(const Vec2f &b) {
    Vec2f tmp;
    tmp.x = this->x + b.x;
    tmp.y = this->y + b.y;
    return tmp;
}

Vec2f Vec2f::operator-(const Vec2f &b) {
    Vec2f tmp;
    tmp.x = this->x - b.x;
    tmp.y = this->y - b.y;
    return tmp;
}

Vec2f Vec2f::operator-() {
    Vec2f tmp;
    tmp.x = -this->x;
    tmp.y = -this->y;
    return tmp;
}

Vec2f Vec2f::operator*(const Vec2f &b) {
    Vec2f tmp;
    tmp.x = this->x * b.x;
    tmp.y = this->y * b.y;
    return tmp;
}

Vec2f Vec2f::operator*(float fac) {
    Vec2f tmp;
    tmp.x = this->x * fac;
    tmp.y = this->y * fac;
    return tmp;
}

Vec2f Vec2f::operator/(float fac) {
    Vec2f tmp;
    tmp.x = this->x / fac;
    tmp.y = this->y / fac;
    return tmp;
}