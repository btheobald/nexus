//
// Created by byron on 30/03/17.
//

#ifndef NEXUS_NODE_H
#define NEXUS_NODE_H

#include "Bound.h"
#include "Body.h"

class Node {
private:
    int firstNew();

public:
    Node* branches[4];
    Bound* nodeBound;
    Body* nodeBody;

    Node(Bound* bound);
    ~Node();

    int insert(Body *b);
    int updateAvg();
};

#endif //NEXUS_NODE_H
