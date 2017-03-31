#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <random>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include "Node.h"
#include "Body.h"

const int displayRes = 700;

bool collide = false;

void integrateTracker(std::vector<Body *> &t, float dT, float G);
void integrateTracker2(std::vector<Body *> &t, Node *root, float dT, float G);
void calculateAccelerationBrute(std::vector<Body *> &t, float G);
void calcForceSingle(Body *b, Node *n, float G);
void calculateAccelerationQuadtree(std::vector<Body *> &t, Node *node, float G);
void treeRecurse(std::vector<Body *> &t, int currentBody, Node *node,  float G);
void printTracker(std::vector<Body *> &t);
void addSuperstructure(std::vector<Body *> &t, int n, float r, float x, float y, float cm, float om, float cs, float G);

void insertTabs(int n);
void printTree(Node *root, int level);

void initDisplay(int lXRes, int lYRes);
GLFWwindow *setupWindow(void);

void drawPoints(std::vector<Body *> &t);
void drawTree(Node *treeRoot, int level);

int main() {
    srand(time(NULL));

    // Setup window and give pointer
    GLFWwindow *window = setupWindow();

    std::vector<Body *> tracker;

    //addSuperstructure(tracker, 1000, 500, -300, -300, 100, 0.1, 100, 1.0);
    //addSuperstructure(tracker, 200, 200, 500, 0, 50, 0.1, 100, 1.0);

    //addSuperstructure(tracker, 1000, 500, 500, 500, 100, 0.01, 50, 1.0);
    //addSuperstructure(tracker, 1000, 500, -500, -500, 100, 0.01, 50, 1.0);

    addSuperstructure(tracker, 2000, 1500, 0, 0, 100, 0.1, 200, 1.0);

    struct timespec start, finish;
    double elapsed;

    while (!glfwWindowShouldClose(window)) {

        clock_gettime(CLOCK_MONOTONIC, &start);

        glClear(GL_COLOR_BUFFER_BIT);

        // Draw Bodies
        drawPoints(tracker);

        // Draw tree
        Node root = Node(new Bound(0, 0, 2000));
        for(int i = 0; i < tracker.size(); i++) {
            root.insert(tracker[i]);
        }
        //drawTree(&root, 0);
        //integrateTracker(tracker, 2.0f, 1.0f);
        integrateTracker2(tracker, &root, 5.0f, 1.0f);

        clock_gettime(CLOCK_MONOTONIC, &finish);

        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1E9;

        std::cout << elapsed << " - " << tracker.size() << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

        /*if(collide) {
            usleep(100000);
            collide = false;
        }*/
    }

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //printTree(&root, 0);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void addSuperstructure(std::vector<Body *> &t, int n, float r, float x, float y, float cm, float om, float cs, float G) {
    // Use Mersenne Twister for RNE within range.
    std::uniform_real_distribution<> pos(0, r*2);
    // Use random device for seed value
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::mt19937 gen; // Use for desktop valgrind - random_device causes segfault
    // Temporary Variables
    double tempRand, tempCirX, tempCirY, tempDist, tempVelX, tempVelY;
    // Add Central Body
    t.push_back(new Body(cm, x, y));
    //int bodyOffset = bodyStore.size() - 1;
    for(int bIDC = 0; bIDC < n; bIDC++) {
        // Ensure that bodies are not too close to center.
        do {
            tempRand = pos(gen) - r;
        } while ((tempRand < cs) & (tempRand > -cs));
        // Map to Circle
        tempCirX = x+(tempRand * std::cos(2 * M_PI * tempRand));
        tempCirY = y+(tempRand * std::sin(2 * M_PI * tempRand));

        // Calculate Distance to Body
        tempDist = std::sqrt(std::pow(x-tempCirX, 2) + std::pow(y-tempCirY, 2));

        // Calc Velocity
        tempVelX = copysign(std::sqrt((G * (cm + om)) / std::pow(tempDist, 3)) * (tempCirY-y), (tempCirY-y));
        tempVelY = copysign(std::sqrt((G * (cm + om)) / std::pow(tempDist, 3)) * (tempCirX-x), -(tempCirX-x));

        t.push_back(new Body(om, tempCirX, tempCirY, tempVelX, tempVelY));
    }
}

void integrateTracker(std::vector<Body *> &t, float dT, float G) {
    calculateAccelerationBrute(t, G);

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdateVel(dT);
    }

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdatePos(dT);
    }

    for(int n = 0; n < t.size(); n++) {
        for(int i = n+1; i < t.size(); i++) {
            float d[] = {t[n]->pos[0] - t[i]->pos[0], t[n]->pos[1] - t[i]->pos[1]};
            float v = std::sqrt(std::pow(d[0],2) + std::pow(d[1],2));
            if (v < t[n]->radius+t[i]->radius) {
                delete t[i];
                t.erase(t.begin()+i);
            }
        }
        if((t[n]->pos[0] > 2000) | (t[n]->pos[0] < -2000) | (t[n]->pos[1] > 2000) | (t[n]->pos[1] < -2000)) {
            delete t[n];
            t.erase(t.begin()+n);
        }
    }
}

void integrateTracker2(std::vector<Body *> &t, Node *root, float dT, float G) {
    calculateAccelerationQuadtree(t, root, G);

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdateVel(dT/2);
    }

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdatePos(dT);
    }

    for(int n = 0; n < t.size(); n++) {
        if((t[n]->pos[0] > 2000) | (t[n]->pos[0] < -2000) | (t[n]->pos[1] > 2000) | (t[n]->pos[1] < -2000)) {
            delete t[n];
            t.erase(t.begin()+n);
        }
    }

    for(int n = 0; n < t.size(); n++) {
        t[n]->simUpdateVel(dT/2);
    }
}

void calculateAccelerationBrute(std::vector<Body *> &t, float G) {
    for(unsigned int i = 0; i < t.size(); i++) {
        t[i]->acc[0] = 0.0;
        t[i]->acc[1] = 0.0;
    }

    for(int a = 0; a < t.size(); a++) {
        for(int b = a+1; b < t.size(); b++) {
            float d[] = { t[a]->pos[0] - t[b]->pos[0], t[a]->pos[1] - t[b]->pos[1] };
            float v = std::sqrt(std::pow(d[0],2) + std::pow(d[1],2));

            float fP = -(G * t[a]->mass * t[b]->mass) / std::pow(v,3);
            float fX = fP * d[0];
            float fY = fP * d[1];

            t[a]->acc[0] += fX / t[a]->mass;
            t[a]->acc[1] += fY / t[a]->mass;
            t[b]->acc[0] -= fX / t[b]->mass;
            t[b]->acc[1] -= fY / t[b]->mass;
        }
    }
}

void calcForceSingle(Body *b, Node *n, float G) {
    float d[] = {b->pos[0] - n->nodeBody->pos[0], b->pos[1] - n->nodeBody->pos[1]};
    float v = std::sqrt(std::pow(d[0],2) + std::pow(d[1],2));

    float fP = -(G * b->mass * n->nodeBody->mass) / std::pow(v,3);
    float fX = fP * d[0];
    float fY = fP * d[1];


    b->acc[0] += fX / b->mass;
    b->acc[1] += fY / b->mass;
}

void treeRecurse(std::vector<Body *> &t, int currentBody, Node *node,  float G) {
    int internal = 0;
    for(int b = 0; b < 4; b++) {
        if(node->branches[b] != nullptr) {
            internal = 1;
        }
    }

    float d[] = {t[currentBody]->pos[0] - node->nodeBody->pos[0], t[currentBody]->pos[1] - node->nodeBody->pos[1]};
    float v = std::sqrt(std::pow(d[0],2) + std::pow(d[1],2));

    if(!internal and node->nodeBody != t[currentBody]) {
        calcForceSingle(t[currentBody], node, G);
        if (v < t[currentBody]->radius+node->nodeBody->radius) {
            //std::cout << "C: " << t[currentBody]->pos.get(0) << " " << t[currentBody]->pos.get(1) << " | " << node->nodeBody->pos.get(0) << " " << node->nodeBody->pos.get(1) << std::endl;

            /*glPointSize(5.0f);
            glColor3b(127, 0, 0);
            glBegin(GL_POINTS);
            glVertex2f(t[currentBody]->pos.get(0), t[currentBody]->pos.get(1));
            glVertex2f(node->nodeBody->pos.get(0), node->nodeBody->pos.get(1));
            collide = true;
            glEnd();
            glColor3b(127, 127, 127);*/

            delete t[currentBody];
            t.erase(t.begin()+currentBody);
        }
        return;
    } else {
        float s = node->nodeBound->getWidth();

        float theta = 0.5;
        float res = s/v;
        if(res < theta) {
            calcForceSingle(t[currentBody], node, G);
            return;
        } else {
            for(int b = 0; b < 4; b++) {
                if(node->branches[b] != nullptr) {
                    treeRecurse(t, currentBody, node->branches[b], G);
                }
            }
        }
    }
}

void calculateAccelerationQuadtree(std::vector<Body *> &t, Node *n, float G) {
    for(int i = 0; i < t.size(); i++) {
        t[i]->acc[0] = 0.0f;
        t[i]->acc[1] = 0.0f;
    }

    for(int a = 0; a < t.size(); a++) {
        treeRecurse(t, a, n, G);
    }
}

void printTracker(std::vector<Body *> &t) {
    for(int n = 0; n < t.size(); n++) {
        std::cout << t[n]->pos[0] << " " << t[n]->pos[1] << "  ";
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
    std::cout << "\\-" << level << "  | m: " << root->nodeBody->mass << " | x: " << root->nodeBody->pos[0] << " | y: " << root->nodeBody->pos[1] << std::endl;
    for (int i = 0; i < 4; i++) {
        if (root->branches[i]) {
            insertTabs(level + 1);
            std::cout << "|->B" << i << std::endl;
            printTree(root->branches[i], level + 1);
        }
    }
}

void initDisplay(int lXRes, int lYRes) {
    // Init Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-lXRes * 3, lXRes * 3, -lYRes * 3, lYRes * 3, 1.0f, -1.0f);

    // Init Modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set Viewport Extents
    glViewport(0, 0, lXRes, lYRes);
    glClearColor(0.0f, 0.0f, 0.0f, 1);
}

GLFWwindow *setupWindow(void) {
    // Try init GLFW
    if (!glfwInit()) {
        fprintf(stderr, "GLFW could not init.\n");
        exit(1);
    }

    // Create window
    GLFWwindow *window =
            glfwCreateWindow(displayRes, displayRes, "QuadTree Example", NULL, NULL);

    // Check that window opened
    if (!window) {
        // Terminate if not
        fprintf(stderr, "GLFW could not create window.\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    initDisplay(displayRes, displayRes);
    glfwSwapInterval(1);

    return window;
}

void drawPoints(std::vector<Body *> &t) {
    // Render center point
    for(int n = 0; n < t.size(); n++) {
        glPointSize(2.0f);
        glColor3b(127, 127, 127);
        glBegin(GL_POINTS);
        glVertex2f(t[n]->pos[0], t[n]->pos[1]);
        glEnd();
        glColor3b(127, 127, 127);
    }
}

void drawTree(Node* treeRoot, int level) {
    int hasBranches = 0;

    float cX = treeRoot->nodeBound->getCenter(0);
    float cY = treeRoot->nodeBound->getCenter(1);
    float hf = treeRoot->nodeBound->getWidth()/2;

    glColor3b(32, 32, 32);
    glBegin(GL_LINE_LOOP);
    glVertex2f(cX - hf, cY + hf);
    glVertex2f(cX + hf, cY + hf);
    glVertex2f(cX + hf, cY - hf);
    glVertex2f(cX - hf, cY - hf);
    glEnd();

    //drawPoints(treeRoot->nodeBounds, level);
    for(int i = 0; i < 4; i++) {
        if(treeRoot->branches[i]) {
            drawTree(treeRoot->branches[i], level+1);
            hasBranches = 1;
        }
    }

    /*glPointSize(2.0f);

    if(hasBranches == 0)
        glColor3b(127, 0, 0);
    else
        glColor3b(0, 64, 0);*/

    /*if(treeRoot->nodeBody) {
        glBegin(GL_POINTS);
        glVertex2d(treeRoot->nodeBody->pos.get(0), treeRoot->nodeBody->pos.get(1));
        glEnd();
    }*/

    glColor3b(127, 127, 127);
}