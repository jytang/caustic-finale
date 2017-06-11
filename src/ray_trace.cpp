#include "ray_trace.h"
#include "Material.h"

#include <iostream>

RayTrace::RayTrace(Scene &s) : scn(s) {}

bool RayTrace::TraceRay(const Ray &ray, Intersection &hit, int depth) {
    if (depth == 0) PrimaryRays++;

    // Find ray intersection
    if (scn.Intersect(ray,hit)==false){
        hit.Shade = scn.GetSkyColor();
        return false;
    }

    // Do lighting (direct illumination)
    int num_lights = scn.GetNumLights();
    Color direct_color = Color::BLACK;
    for (unsigned int i = 0; i < num_lights; ++i)
    {
        Light &l = scn.GetLight(i);
        Color lightColor;
        glm::vec3 toLight;
        glm::vec3 ltPos;
        float intensity = l.Illuminate(hit.Position, lightColor, toLight, ltPos);
        if (intensity == 0) {
            std::cerr << "shouldnt happen" << std::endl;
            continue;
        }
        // Does hit normal face light?
        if (glm::dot(toLight, hit.Normal) < 0) continue;

        // Check shadows for this light.
        Ray shadowRay;
        shadowRay.Direction = toLight;
        shadowRay.Origin = hit.Position;
        Intersection shadowHit;
        ShadowRays++;
        // TODO: Short circuit intersection code ("ShadowIntersect")
        if (scn.Intersect(shadowRay, shadowHit)) {
            if (shadowHit.HitDistance < glm::distance(hit.Position,ltPos))
                continue; // go to next light
        }

        Color c;
        hit.Mtl->ComputeReflectance(c, -ray.Direction, toLight, hit);
        c.Multiply(lightColor);
        c.Scale(intensity);

        direct_color.Add(c);
    }

    // Don't bounce if at max depth.
    hit.Shade = direct_color;
    if (depth == MaxDepth) return true;

    // Indirect illumination
    SecondaryRays++;
    glm::vec3 outDir; // trace new ray in this dir
    Color outColor; // material color
    hit.Mtl->GenerateSample(hit, ray.Direction, outDir, outColor);

    if (glm::dot(outDir, hit.Normal) < 0) return true;

    // Trace secondary ray
    Ray secondaryRay;
    secondaryRay.Direction = outDir;
    secondaryRay.Origin = hit.Position;
    Intersection secondaryHit;
    TraceRay(secondaryRay, secondaryHit, depth+1); // returns bool but we don't care;

    // Scale returned color from secondary ray by out color of material
    Color indirect_color = secondaryHit.Shade;
    indirect_color.Multiply(outColor);
    hit.Shade.Add(indirect_color);

    return true;
}
