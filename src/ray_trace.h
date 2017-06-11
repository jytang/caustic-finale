#pragma once

#include "Scene.h"
#include "Intersection.h"
#include "Ray.h"

class RayTrace {
public:
    RayTrace(Scene &s);

    bool TraceRay(const Ray &ray, Intersection &hit, int depth=0);
private:
    Scene &scn;

    // Settings
    int MaxDepth = 10;

    // Statistics
    int PrimaryRays;
    int SecondaryRays;
    int ShadowRays;
};
