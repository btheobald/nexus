//
// Created by byron on 30/03/17.
//

#ifndef NEXUS_BODY_H
#define NEXUS_BODY_H

class Body {
public:
    float mass;
    float radius;
    float pos[2];
    float vel[2];
    float acc[2];

    Body();
    Body(float _m, float _x, float _y, float _vx=0.0f, float _vy=0.0f);
    ~Body();

    void simUpdatePos(float dt);
    void simUpdateVel(float dt);
};

#endif //NEXUS_BODY_H
