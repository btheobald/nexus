//
// Created by byron on 30/03/17.
//

#include <iostream>
#include "Node.h"

Node::Node(Bound *bound) {
    for(int i = 0; i < 4; i++) {
        branches[i] = nullptr;
    }
    nodeBound = bound;
    nodeBody = nullptr;
}

Node::~Node() {
    for(int i = 0; i < 4; i++) {
        if(branches[i] != nullptr) {
            delete branches[i];
        }
    }
    delete nodeBody;
    delete nodeBound;
}

int Node::insert(Body *b) {
    if(nodeBody == nullptr) {
        nodeBody = b;
        return 0;
    } else {
        int nQuad = nodeBound->getQuadrant(b->pos);
        if((nodeBody->pos == b->pos) & (nodeBody->mass != -1)) {
            // If bodies are in the same position abort.
            return -1;
        }
        if(branches[nQuad] != nullptr) {
            branches[nQuad]->insert(b);
        } else {
            branches[nQuad] = new Node(nodeBound->getNewBound(nQuad));
            branches[nQuad]->insert(b);

            if(firstNew()) {
                Body* tmp = nodeBody;
                nodeBody = new Body(-1, 0, 0);
                insert(tmp);
            }
        }
        updateAvg();
    }
    return 0;
}

int Node::firstNew() {
    // Check if first new quadrant
    int check = 0;
    for(int n = 0; n < 4; n++) {
        if(branches[n] != nullptr) {
            check++;
        }
    }

    if(check == 1) return 1; // If first new quadrant

    return 0; // If not first
}

int Node::updateAvg() {
    int hasBranch = 0;
    for(int b = 0; b < 4; b++) {
        if(branches[b] != nullptr) {
            hasBranch = 1;
        }
    }
    if(!hasBranch) {
        return 0;
    }

    nodeBody->mass = 0;
    nodeBody->pos.set(0.0f, 0.0f);

    for(int b = 0; b < 4; b++) {
        if(branches[b] != nullptr) {
            if(branches[b]->nodeBody != nullptr) {
                branches[b]->updateAvg();
                Vec2f wA = nodeBody->pos * nodeBody->mass;
                Vec2f wB = branches[b]->nodeBody->pos * branches[b]->nodeBody->mass;
                float mT = nodeBody->mass + branches[b]->nodeBody->mass;

                nodeBody->pos = ((wA + wB)/mT);
                nodeBody->mass = mT;
            }
        }
    }
}