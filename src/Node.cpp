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
            std::cout << "X" << std::endl;
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