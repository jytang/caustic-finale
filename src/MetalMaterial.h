////////////////////////////////////////
// MetalMaterial.h
////////////////////////////////////////

#pragma once

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class MetalMaterial:public Material {
public:
    void SetColor(Color c) {
        DiffuseColor = c;
    }
    void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit);
    void GenerateSample(const Intersection &isect, const glm::vec3 &inDir, glm::vec3 &outDir, Color &outColor);

private:
    Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
