#include "SphereObject.h"
#include <glm/gtx/norm.hpp>

SphereObject::SphereObject() {}

bool SphereObject::Intersect(const Ray &ray, Intersection &hit)
{
    float t = -1 * (glm::dot(ray.Origin - center, ray.Direction));
    glm::vec3 q = ray.Origin + t * ray.Direction;

    float dist_squared = glm::distance2(q, center);
    float radius_squared = radius * radius;

    // Outside of sphere
    if (dist_squared > radius_squared) return false;

    glm::vec3 hit_point;
    float dist_from_radius = glm::sqrt(radius_squared - dist_squared);
    glm::vec3 q1 = ray.Origin + (t-dist_from_radius) * ray.Direction;
    glm::vec3 q2 = ray.Origin + (t+dist_from_radius) * ray.Direction;

    if (t-dist_from_radius >= 0)
    {
        hit_point = q1;
        t = t-dist_from_radius;
    } else if (t+dist_from_radius >= 0)
    {
        hit_point = q2;
        t = t+dist_from_radius;
    } else {
        return false;
    }

    // Hits sphere at point hit_point, with distance t.
    if (t < hit.HitDistance)
    {
        hit.Position = hit_point;
        hit.Normal = glm::normalize((hit_point-center)/radius);
        hit.HitDistance = t;
        hit.Mtl = Mtl;
        return true;
    }

    return false;
}
