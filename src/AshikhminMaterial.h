////////////////////////////////////////
// AshikhminMaterial.h
////////////////////////////////////////

#pragma once

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class AshikhminMaterial:public Material {
public:
    void SetDiffuseColor(Color c);
    void SetSpecularColor(Color c);
    void SetDiffuseLevel(float level);
    void SetSpecularLevel(float level);
    void SetRoughness(float u, float v);

    void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit);
    void GenerateSample(const Intersection &isect, const glm::vec3 &inDir, glm::vec3 &outDir, Color &outColor);

private:
    Color DiffuseColor = Color::BLACK;
    Color SpecularColor = Color::BLACK;
    float rd = 0;
    float rs = 0;
    float nu = 0;
    float nv = 0;
};

////////////////////////////////////////////////////////////////////////////////
