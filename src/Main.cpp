////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "MeshObject.h"
#include "InstanceObject.h"
#include "PlaneObject.h"
#include "SphereObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "finale.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

void project1();
void spheres();

////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
    finale();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void project1() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

    // Create boxes
    MeshObject box1;
    box1.MakeBox(5.0f,0.1f,5.0f);
    scn.AddObject(box1);

    MeshObject box2;
    box2.MakeBox(1.0f,1.0f,1.0f);

    InstanceObject inst1;
    inst1.SetChild(box2);
    glm::mat4x4 mtx=glm::rotate(glm::mat4x4(),0.5f,glm::vec3(1,0,0));
    mtx[3][1]=1.0f;
    inst1.SetMatrix(mtx);
    scn.AddObject(inst1);

    InstanceObject inst2;
    inst2.SetChild(box2);
    mtx=glm::rotate(glm::mat4x4(),1.0f,glm::vec3(0,1,0));
    mtx[3]=glm::vec4(-1,0,1,1);
    inst2.SetMatrix(mtx);
    scn.AddObject(inst2);

    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(0.5f);
    sunlgt.SetDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
    scn.AddLight(sunlgt);

    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
    redlgt.SetIntensity(2.0f);
    redlgt.SetPosition(glm::vec3(2.0f, 2.0f, 0.0f));
    scn.AddLight(redlgt);

    // Create camera
    RayTrace rt(scn);
    Camera cam(rt);
    cam.LookAt(glm::vec3(2.0f,2.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0,1,0));
    cam.SetResolution(800,600);
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);

    // Render image
    cam.Render();
    cam.SaveBitmap("project1.bmp");
}

void spheres()
{
    Scene scn;
    scn.shadows_on = true;
    scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));
    // Create ground plane
    PlaneObject ground;
    scn.AddObject(ground);
    // Create spheres
    for(int i=0;i<20;i++) {
        SphereObject *sphere=new SphereObject;
        float rad=glm::linearRand(0.25f,0.5f);
        glm::vec3 pos(glm::linearRand(-5.0f,5.0f),rad,glm::linearRand(-5.0f,5.0f));
        sphere->SetRadius(rad);
        sphere->SetCenter(pos);
        scn.AddObject(*sphere);
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
    cam.LookAt(glm::vec3(-0.75f,0.25f,5.0f),glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.f, 1.f, 0.f));
    cam.SetResolution(800,600);
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);

    // Render image
    cam.Render();
    cam.SaveBitmap("spheres.bmp");
}

////////////////////////////////////////////////////////////////////////////////
