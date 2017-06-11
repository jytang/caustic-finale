#pragma once

#include "Object.h"
#include "MeshObject.h"
#include "box_tree_node.h"

class BoxTree : public Object {
public:
    BoxTree();
    ~BoxTree();
    void Construct(MeshObject &o);
    bool Intersect(const Ray &ray, Intersection &hit) override;
private:
    BoxTreeNode *root;
};
