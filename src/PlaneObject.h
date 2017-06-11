#pragma once

#include "Object.h"

class PlaneObject : public Object{
public:
	PlaneObject();
	bool Intersect(const Ray &ray, Intersection &hit);
    glm::vec3 GetNormal() { return normal; }
    float GetDistance() { return distance; }
private:
    glm::vec3 normal;
    float distance;
	Material *Mtl;
};
