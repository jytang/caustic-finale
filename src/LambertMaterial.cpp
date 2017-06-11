#include "LambertMaterial.h"

#include <cstdlib>
#include <iostream>

void LambertMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
    col = DiffuseColor;
}

void LambertMaterial::GenerateSample(const Intersection &isect,
        const glm::vec3 &inDir,
        glm::vec3 &outDir,
        Color &outColor)
{
    glm::vec3 normal = isect.Normal;

    // Cosine-weighted hemisphere.
    float s = ((float) rand()) / RAND_MAX;
    float t = ((float) rand()) / RAND_MAX;
    float pi=3.1415926f;
    float u = 2.f * pi * s;
    float v = glm::sqrt(1.f-t);

    float x_comp = v * glm::cos(u);
    float y_comp = glm::sqrt(t);
    float z_comp = v * glm::sin(u);

    // Create orthogonal basis
    glm::vec3 j = normal;
    glm::vec3 arbitrary = glm::normalize(glm::vec3(((float) rand()) / RAND_MAX,
            ((float) rand()) / RAND_MAX, ((float) rand()) / RAND_MAX));
    glm::vec3 i = glm::normalize(glm::cross(j, arbitrary));
    glm::vec3 k = glm::normalize(glm::cross(j, i));

    outDir = glm::normalize(x_comp * i + y_comp * j + z_comp * k);
    outColor = DiffuseColor;
}
