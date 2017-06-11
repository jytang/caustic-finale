#pragma once

#define MAX_TRIANGLES_PER_BOX 10

#include "Ray.h"
#include "Intersection.h"
#include "Triangle.h"

class BoxTreeNode {
public:
    BoxTreeNode();
    ~BoxTreeNode();
    bool Intersect(const Ray &ray, Intersection &hit);
    void Construct(int count, Triangle **tri);

    static bool volume_intersect_helper(BoxTreeNode *n, const Ray &ray, Intersection &hit);

    BoxTreeNode *left, *right;
    glm::vec3 box_min, box_max;
private:
    int count;
    Triangle *tri[MAX_TRIANGLES_PER_BOX];
};
