#include "MetalMaterial.h"

void MetalMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
    float pi=3.1415926f;
    col.Scale(DiffuseColor,0);
}

void MetalMaterial::GenerateSample(const Intersection &isect,
        const glm::vec3 &inDir,
        glm::vec3 &outDir,
        Color &outColor)
{
    glm::vec3 normal = isect.Normal;
    outDir = glm::normalize(inDir - 2.f * glm::dot(inDir, normal) * normal);

    outColor = DiffuseColor;
}
