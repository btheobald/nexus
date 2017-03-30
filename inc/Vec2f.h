//
// Created by byron on 30/03/17.
//

#ifndef NEXUS_VEC2F_H
#define NEXUS_VEC2F_H

class Vec2f {
private:
    float x;
    float y;

public:
    Vec2f(float x = 0.0f, float y = 0.0f); // Inits to param
    Vec2f(Vec2f *c); // Inits to existing (copy)

    // Get and Set
    float get(int xy);
    void set(float x, float y);

    // Overloaded Operators
    void operator+=(const Vec2f& b);
    Vec2f operator+(const Vec2f& b);
    Vec2f operator*(float fac);
};


#endif //NEXUS_VEC2F_H
