#pragma once

#include "Object.h"

class SphereObject : public Object{
public:
	SphereObject();
	bool Intersect(const Ray &ray, Intersection &hit);
    void SetRadius(float radius) { this->radius = radius; }
    void SetCenter(glm::vec3 center) { this->center = center; }
    float GetRadius() { return radius; }
    glm::vec3 GetCenter() { return center; }
private:
    float radius;
    glm::vec3 center;
	Material *Mtl;
};
