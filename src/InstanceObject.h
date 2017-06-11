#pragma once

#include "Object.h"
#include "Material.h"

class InstanceObject : public Object{
public:
	InstanceObject();
	bool Intersect(const Ray &ray, Intersection &hit);
	void SetChild(Object &obj);
	void SetMatrix(glm::mat4x4 &mtx);
    void SetMaterial(Material *m);
private:
    Material *mat;
	glm::mat4x4 Matrix;
	glm::mat4x4 Inverse; // Pre-computed inverse of Matrix
	Object *Child;
};
