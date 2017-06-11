////////////////////////////////////////
// MeshObject.h
////////////////////////////////////////

#pragma once

#include "Object.h"
#include "Triangle.h"

////////////////////////////////////////////////////////////////////////////////

class MeshObject:public Object {
public:
	MeshObject();
	~MeshObject();

	bool Intersect(const Ray &ray,Intersection &hit);

	void MakeBox(float x,float y,float z,Material *mtl=0);

    bool LoadPLY(const char *filename, Material *mtl=0);

	void Smooth();

	int NumVertexes,NumTriangles;

	Triangle *Triangles;

private:
	Vertex *Vertexes;
};

////////////////////////////////////////////////////////////////////////////////
