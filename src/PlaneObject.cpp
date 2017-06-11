#include "PlaneObject.h"

PlaneObject::PlaneObject() {
    normal = glm::vec3(0.f, 1.f, 0.f);
    distance = 0.f;
}

bool PlaneObject::Intersect(const Ray &ray, Intersection &hit)
{
    float denom = glm::dot(ray.Direction,normal);
    if (denom == 0) return false;
    float t = (distance-glm::dot(ray.Origin,normal)) / denom;

    if (t >= 0 && t < hit.HitDistance)
    {
        hit.Position = ray.Origin + t * ray.Direction;
        hit.Normal = glm::normalize(normal);
        hit.HitDistance = t;
        hit.Mtl = Mtl;
        return true;
    }

    return false;
}
