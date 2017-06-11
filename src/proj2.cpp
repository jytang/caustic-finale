#include "proj2.h"

#include "MeshObject.h"
#include "InstanceObject.h"
#include "PlaneObject.h"
#include "SphereObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "box_tree.h"
#include "ray_trace.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

void proj2() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));

    // Create ground
    MeshObject ground;
    ground.MakeBox(5.0f,0.1f,5.0f);
    scn.AddObject(ground);

    // Create dragon
    MeshObject dragon;
    dragon.LoadPLY("dragon.ply");
    dragon.Smooth();
    BoxTree tree;
    tree.Construct(dragon);
    scn.AddObject(tree);

    // Create instance
    InstanceObject inst;
    inst.SetChild(tree);
    const float PI = 3.1415926;
    glm::mat4x4 mtx=glm::eulerAngleY(PI);
    mtx[3]=glm::vec4(-0.05f,0.0f,-0.1f,1.0f);
    inst.SetMatrix(mtx);
    scn.AddObject(inst);

    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
    redlgt.SetIntensity(0.02f);
    redlgt.SetPosition(glm::vec3(-0.2f, 0.2f, 0.2f));
    scn.AddLight(redlgt);
    PointLight bluelgt;
    bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f));
    bluelgt.SetIntensity(0.02f);
    bluelgt.SetPosition(glm::vec3(0.1f, 0.1f, 0.3f));
    scn.AddLight(bluelgt);

    // Create camera
    RayTrace rt(scn);
    Camera cam(rt);
    cam.LookAt(glm::vec3(-0.1f,0.1f,0.2f),glm::vec3(-0.05f,0.12f,0.0f),
            glm::vec3(0,1.0f,0));
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.SetResolution(800,600);

    // Render image
    cam.Render();
    cam.SaveBitmap("project2.bmp");
}
