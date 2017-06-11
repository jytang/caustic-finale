#include "box_tree_node.h"

#include <iostream>
BoxTreeNode::BoxTreeNode() {}
BoxTreeNode::~BoxTreeNode() {}

bool BoxTreeNode::volume_intersect_helper(BoxTreeNode *n, const Ray &ray, Intersection &hit) {
    // check that it is inside volume
    if (ray.Origin.x <= n->box_max.x &&
        ray.Origin.x >= n->box_min.x &&
        ray.Origin.y <= n->box_max.y &&
        ray.Origin.y >= n->box_min.y &&
        ray.Origin.z <= n->box_max.z &&
        ray.Origin.z >= n->box_min.z) {
        hit.HitDistance = -1.f; // closest volume obviously
        return true;
    }

    // Check intersection of ray with self
    float tx1 = (n->box_min.x - ray.Origin.x) / ray.Direction.x;
    float tx2 = (n->box_max.x - ray.Origin.x) / ray.Direction.x;
    float ty1 = (n->box_min.y - ray.Origin.y) / ray.Direction.y;
    float ty2 = (n->box_max.y - ray.Origin.y) / ray.Direction.y;
    float tz1 = (n->box_min.z - ray.Origin.z) / ray.Direction.z;
    float tz2 = (n->box_max.z - ray.Origin.z) / ray.Direction.z;
    float tmin = glm::max( glm::max( glm::min(tx1,tx2), glm::min(ty1,ty2) ), glm::min(tz1,tz2));
    float tmax = glm::min( glm::min( glm::max(tx1,tx2), glm::max(ty1,ty2) ), glm::max(tz1,tz2));

    if (tmin > tmax || tmax < 0.0001f) return false; // doesn't hit this box. stop.

    if (tmin < 0)
        hit.HitDistance = tmax;
    else
        hit.HitDistance = tmin;
    return true;
}

bool BoxTreeNode::Intersect(const Ray &ray, Intersection &hit) {
    // Check if leaf node
    if (count <= MAX_TRIANGLES_PER_BOX) {
        bool success = false;
        for (unsigned int i = 0; i < count; ++i) {
            if (tri[i]->Intersect(ray, hit))
                success = true;
        }
        return success;
    }

    // Test left and right volumes.
    Intersection volhit1;
    bool hit1 = volume_intersect_helper(left, ray, volhit1);
    Intersection volhit2;
    bool hit2 = volume_intersect_helper(right, ray, volhit2);

    if (hit1 && hit2) { // both hit
        bool success = false;
        if (left->Intersect(ray, hit))
            success = true;
        if (right->Intersect(ray, hit))
            success = true;
        return success;
    }
    else if (hit1) // only left volume hit
        return  left->Intersect(ray, hit);
    else if (hit2) // only right volume hit
        return right->Intersect(ray, hit);
    else // neither hit
        return false;
}

void BoxTreeNode::Construct(int count, Triangle **ts) {
    this->count = count;

    // Compute BoxMin & BoxMax to fit around all tri's
    float minx, miny, minz;
    float maxx, maxy, maxz;
    glm::vec3 pos = ts[0]->GetVtx(0).Position;
    minx = maxx = pos.x;
    miny = maxy = pos.y;
    minz = maxz = pos.z;

    for (unsigned int i = 0; i < count; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            pos = ts[i]->GetVtx(j).Position;
            minx = glm::min(minx, pos.x);
            miny = glm::min(miny, pos.y);
            minz = glm::min(minz, pos.z);
            maxx = glm::max(maxx, pos.x);
            maxy = glm::max(maxy, pos.y);
            maxz = glm::max(maxz, pos.z);
        }
    }
    box_min = {minx, miny, minz};
    box_max = {maxx, maxy, maxz};

    // Check if this is a leaf node
    if (count <= MAX_TRIANGLES_PER_BOX) {
        // Add all triangles to array and return.
        for (int i = 0; i < count; ++i) {
            tri[i] = ts[i];
        }
        return;
    }

    // Determine largest box dimension x,y or z
    float xlen = maxx-minx;
    float ylen = maxy-miny;
    float zlen = maxz-minz;

    // Compute splitting plane
    int dim; // 0=x, 1=y, 2=z
    float splitpt;
    if (xlen >= ylen && xlen >= zlen) {
        dim = 0;
        splitpt = minx + (xlen / 2.f);
    } else if (ylen >= xlen && ylen >= zlen) {
        dim = 1;
        splitpt = miny + (ylen / 2.f);
    } else if (zlen >= xlen && zlen >= ylen) {
        dim = 2;
        splitpt = minz + (zlen / 2.f);
    }

    // Allocate two new temporary arrays
    Triangle **tri1 = new Triangle*[count];
    Triangle **tri2 = new Triangle*[count];
    int count1=0, count2=0;

    // Place triangles into group 1 or group 2
    for (int i = 0; i < count; ++i) {
        // Compute center of triangle and determine which side of splitting plane.
        glm::vec3 center = {0.f, 0.f, 0.f};
        for (int j = 0; j < 3; ++j) {
            center += ts[i]->GetVtx(j).Position;
        }
        center /= 3.f;
        // Add to appropriate group
        if (center[dim] < splitpt) {
            tri1[count1] = ts[i];
            count1++;
        } else {
            tri2[count2] = ts[i];
            count2++;
        }
    }

    // Check if either group is empty. If so, move 1 triangle into that group.
    if (count1 == 0) {
        tri1[count1] = tri2[count2-1];
        count1++;
        count2--;
    } else if (count2 == 0) {
        tri2[count2] = tri1[count1-1];
        count2++;
        count1--;
    }

    // Recursively build sub-trees
    left = new BoxTreeNode();
    left->Construct(count1, tri1);

    right = new BoxTreeNode();
    right->Construct(count2, tri2);

    // Free up arrays
    delete []tri1;
    delete []tri2;
}
