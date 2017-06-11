#include "proj3.h"

#include "MeshObject.h"
#include "InstanceObject.h"
#include "PlaneObject.h"
#include "SphereObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "LambertMaterial.h"
#include "MetalMaterial.h"
#include "box_tree.h"
#include "ray_trace.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

void proj3() {
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f,0.9f,1.0f));

	// Create ground
	LambertMaterial groundMtl;
	groundMtl.SetColor(Color(0.25f,0.25f,0.25f));
	MeshObject ground;
	ground.MakeBox(2.0f,0.11f,2.0f,&groundMtl);
	scn.AddObject(ground);

	// Load dragon mesh
	MeshObject dragon;
	dragon.LoadPLY("dragon.ply");

	// Create box tree
	BoxTree tree;
	tree.Construct(dragon);

	// Materials
	LambertMaterial white;
	white.SetColor(Color(0.7f,0.7f,0.7f));
	LambertMaterial red;
	red.SetColor(Color(0.7f,0.1f,0.1f));
	MetalMaterial metal;
	metal.SetColor(Color(0.95f,0.64f,0.54f));
	const int numDragons=4;
	Material *mtl[numDragons]={&white,&metal,&red,&white};

	// Create dragon instances
	glm::mat4 mtx;
	for(int i=0;i<numDragons;i++) {
		InstanceObject *inst=new InstanceObject();
		inst->SetChild(tree);
		mtx[3]=glm::vec4(0.0f,0.0f,0.3f*(float(i)/float(numDragons-1)-0.5f),1.0f);
		inst->SetMatrix(mtx);
		inst->SetMaterial(mtl[i]);
		scn.AddObject(*inst);
	}

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(1.0f);
	sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);

	// Create camera
	RayTrace rt(scn);
	Camera cam(rt);
	cam.SetResolution(640,480);
	cam.SetAspect(1.33f);
	cam.LookAt(glm::vec3(-0.5f,0.25f,-0.2f),glm::vec3(0.0f,0.15f,0.0f),
			glm::vec3(0,1.0f,0));
	cam.SetFOV(40.0f);
	cam.SetSuperSample(10,10);
	cam.SetJitter(true);
	cam.SetShirley(true);

	// Render image
	cam.Render();
	cam.SaveBitmap("project3.bmp");
}
