#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <random>
#include "Node.h"
#include "Body.h"

void integrateTracker(std::vector<Body *> &t, float dT, float G);
void integrateTracker2(std::vector<Body *> &t, Node *root, float dT, float G);
void calculateAccelerationBrute(std::vector<Body *> &t, float G);
void calcForceSingle(Body *b, Node *n, float G);
void calculateAccelerationQuadtree(std::vector<Body *> &t, Node *node, float G);
void treeRecurse(Body *currentBody, Node *n, float G);
void printTracker(std::vector<Body *> &t);

void insertTabs(int n);
void printTree(Node *root, int level);

int main() {
    srand(time(NULL));

    std::vector<Body *> tracker;
    /*tracker.push_back(new Body(1.0, -40.0, 35.0));
    tracker.push_back(new Body(2.0, 20.0, 45.0));
    tracker.push_back(new Body(3.0, 5.0, 10.0));
    tracker.push_back(new Body(4.0, 45.0, 15.0));
    tracker.push_back(new Body(5.0, 35.0, -40.0));*/

    // Use Mersenne Twister for RNE within range.
    std::uniform_real_distribution<> pos(0, 1000*2);
    // Use random device for seed value
    std::random_device r;
    std::mt19937 gen(r());
    //std::mt19937 gen; // Use for desktop valgrind - random_device causes segfault
    // Temporary Variables
    double tempRand, tempCirX, tempCirY, tempDist, tempVelX, tempVelY;
    // Add Central Body
    tracker.push_back(new Body(100, 0, 0));
    //int bodyOffset = bodyStore.size() - 1;
    for(int bIDC = 0; bIDC < 1000; bIDC++) {
        // Ensure that bodies are not too close to center.
        do {
            tempRand = pos(gen) - 1000;
        } while ((tempRand < 100) & (tempRand > -100));
        // Map to Circle
        tempCirX = (tempRand * std::cos(2 * M_PI * tempRand));
        tempCirY = (tempRand * std::sin(2 * M_PI * tempRand));

        // Calculate Distance to Body
        tempDist = std::sqrt(std::pow(tempCirX, 2) + std::pow(tempCirY, 2));

        // Calc Velocity
        tempVelX = copysign(std::sqrt((1 * (100 + 0.1)) / std::pow(tempDist, 3)) * (tempCirY), (tempCirY));
        tempVelY = copysign(std::sqrt((1 * (100 + 0.1)) / std::pow(tempDist, 3)) * (tempCirX), -(tempCirX));

        tracker.push_back(new Body(0.1, tempCirX, tempCirY, tempVelX, tempVelY));
    }

    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    /*for(int i = 0; i < 100; i++) {
        integrateTracker(tracker, 2.0f, 1.0f);
        printTracker(tracker);
    }*/

    for(int i = 0; i < 100; i++) {
        Node root = Node(new Bound(0, 0, 3000));
        for(int i = 0; i < tracker.size(); i++) {
            root.insert(tracker[i]);
        }
        integrateTracker2(tracker, &root, 2.0f, 1.0f);
        //printTracker(tracker);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    std::cout << elapsed << std::endl;

    //printTree(&root, 0);

    return 0;
}

void integrateTracker(std::vector<Body *> &t, float dT, float G) {
    calculateAccelerationBrute(t, G);

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdateVel(dT);
    }

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdatePos(dT);
    }
}

void integrateTracker2(std::vector<Body *> &t, Node *root, float dT, float G) {
    calculateAccelerationQuadtree(t, root, G);

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdateVel(dT);
    }

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdatePos(dT);
    }
}

void calculateAccelerationBrute(std::vector<Body *> &t, float G) {
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

void calcForceSingle(Body *b, Node *n, float G) {
    Vec2f d = b->pos - n->nodeBody->pos;
    float v = std::sqrt(std::pow(d.get(0),2) + std::pow(d.get(1),2));

    float fP = -(G * b->mass * n->nodeBody->mass) / std::pow(v,3);
    Vec2f fXY = d * fP;

    b->acc += fXY / b->mass;
}

void treeRecurse(Body *currentBody, Node *node, float G) {
    int internal = 0;
    for(int b = 0; b < 4; b++) {
        if(node->branches[b] != nullptr) {
            internal = 1;
        }
    }
    if(!internal and node->nodeBody != currentBody) {
        calcForceSingle(currentBody, node, G);
        return;
    } else {
        Vec2f d = currentBody->pos - node->nodeBody->pos;
        float v = std::sqrt(std::pow(d.get(0),2) + std::pow(d.get(1),2));
        float s = node->nodeBound->getWidth();

        float theta = 0.5;
        float res = s/v;
        if(res < theta) {
            calcForceSingle(currentBody, node, G);
            return;
        } else {
            for(int b = 0; b < 4; b++) {
                if(node->branches[b] != nullptr) {
                    treeRecurse(currentBody, node->branches[b], G);
                }
            }
        }
    }
}

void calculateAccelerationQuadtree(std::vector<Body *> &t, Node *n, float G) {
    for(int i = 0; i < t.size(); i++) {
        t[i]->acc.set(0.0f, 0.0f);
    }

    for(int a = 0; a < t.size(); a++) {
        treeRecurse(t[a], n, G);
    }
}

void printTracker(std::vector<Body *> &t) {
    for(int n = 0; n < t.size(); n++) {
        std::cout << t[n]->pos.get(0) << " " << t[n]->pos.get(1) << "  ";
    }
    std::cout << std::endl;
}

void insertTabs(int n) {
    for(int l = 0; l < n; l++) {
        std::cout << "  ";
    }
}

void printTree(Node *root, int level) {
    insertTabs(level);
    std::cout << "\\-" << level << "  | m: " << root->nodeBody->mass << " | x: " << root->nodeBody->pos.get(0) << " | y: " << root->nodeBody->pos.get(1) << std::endl;
    for (int i = 0; i < 4; i++) {
        if (root->branches[i]) {
            insertTabs(level + 1);
            std::cout << "|->B" << i << std::endl;
            printTree(root->branches[i], level + 1);
        }
    }
}