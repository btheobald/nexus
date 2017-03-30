#include <iostream>
#include <vector>
#include <cmath>
#include "Body.h"

void integrateTracker(std::vector<Body *> &t, float dT, float G);
void calculateAcceleration(std::vector<Body *> &t, float G);
void printTracker(std::vector<Body *> &t);

int main() {
    std::vector<Body *> tracker;
    // Init tracker vector with simple orbital configuration
    tracker.push_back(new Body(100.0, 0.0, 0.0));
    tracker.push_back(new Body(1.0, 10.0, 0.0, 0.0, 1.0));

    for(int i = 0; i < 50; i++) {
        integrateTracker(tracker, 1.0f, 0.1f);
        printTracker(tracker);
    }
}

void integrateTracker(std::vector<Body *> &t, float dT, float G) {
    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdateVel(dT/2);
    }

    calculateAcceleration(t, G);

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdatePos(dT);
    }

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdateVel(dT/2);
    }
}

void calculateAcceleration(std::vector<Body *> &t, float G) {
    for(unsigned int i = 0; i < t.size(); i++) {
        t[i]->acc.set(0.0f, 0.0f);
    }

    for(int a = 0; a < t.size(); a++) {
        for(int b = a+1; b < t.size(); b++) {
            Vec2f d = t[a]->pos - t[b]->pos;
            float v = std::sqrt(std::pow(d.get(0),2) + std::pow(d.get(1),2));

            float fP = -(G * t[a]->mass * t[b]->mass) / std::pow(v,3);
            Vec2f fXY = d * fP;

            //std::cout << fXY.get(0) << " " << fXY.get(1) << std::endl;

            t[a]->acc += fXY / t[a]->mass;
            t[b]->acc += -(fXY) / t[b]->mass;
        }
    }
}

void printTracker(std::vector<Body *> &t) {
    for(int n = 0; n < t.size(); n++) {
        std::cout << t[n]->pos.get(0) << " " << t[n]->pos.get(1) << "  ";
    }
    std::cout << std::endl;
}