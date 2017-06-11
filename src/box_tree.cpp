#include "box_tree.h"

#include <iostream>
#include <cstdio>
#include <ctime>

BoxTree::BoxTree() {}

BoxTree::~BoxTree() {
    //if (root) delete root;
}

bool BoxTree::Intersect(const Ray &ray, Intersection &hit) {
    Intersection tempHit;
    if (BoxTreeNode::volume_intersect_helper(root, ray, tempHit))
        return root->Intersect(ray, hit);
    else
        return false;
}

void BoxTree::Construct(MeshObject &o) {
    // START TIMER.
    std::clock_t start;
    double duration;
    start = std::clock();
    std::cout << "Starting construct..." << std::endl;

    root = new BoxTreeNode();
    Triangle ** pass = new Triangle*[o.NumTriangles];
    for (unsigned int i = 0; i < o.NumTriangles; ++i) {
        pass[i] = &(o.Triangles[i]);
    }
    root->Construct(o.NumTriangles, pass);

    // REPORT TIMER.
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << "Construct duration: " << duration << std::endl;
}
