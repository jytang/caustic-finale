#include "AshikhminMaterial.h"

#include <iostream>
#include <cstdlib>

void AshikhminMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
    col = Color::BLACK;

    float pi=3.1415926f;
    glm::vec3 h = glm::normalize(in + out);
    glm::vec3 n = glm::normalize(hit.Normal);
    glm::vec3 utan = glm::normalize(hit.TangentU);
    glm::vec3 vtan = glm::normalize(hit.TangentV);
    float k_dot_h = glm::dot(in,h);
    float n_dot_in = glm::dot(n,in);
    float n_dot_out = glm::dot(n,out);
    float n_dot_h = glm::dot(n,h);
    float u_dot_h = glm::dot(utan,h);
    float v_dot_h = glm::dot(vtan,h);

    /* SPECULAR TERM */
    // SPECULAR SCALAR FACTOR
    float specular_term   = glm::sqrt( (nu + 1) * (nv + 1) ) / (8 * pi);
    specular_term        *= glm::pow(n_dot_h, ((nu*glm::pow(u_dot_h, 2)) + (nv*glm::pow(v_dot_h, 2))) / (1 - glm::pow(n_dot_h, 2)));
    specular_term        /= k_dot_h * glm::max( n_dot_in, n_dot_out) ;

    // FRESNEL FRACTION (SCHLICK APPROXIMATION)
    specular_term        *= rs + (1 - rs) * glm::pow((1 - k_dot_h), 5);

    // Scale specular color by specular term.
    Color specular_component = SpecularColor;
    specular_component.Scale(specular_term);

    /* DIFFUSE TERM */
    float diffuse_term = (28.f * rd) / (15.f * pi);
    diffuse_term      *= (1.f - rs);
    diffuse_term      *= (1 - glm::pow( (1-(n_dot_in / 2.f)), 5));
    diffuse_term      *= (1 - glm::pow( (1-(n_dot_out / 2.f)), 5));

    // Scale diffuse color by diffuse term.
    Color diffuse_component = DiffuseColor;
    diffuse_component.Scale(diffuse_term);

    col.Add(diffuse_component);
    //std::cerr << "r: " << diffuse_component.Red << "  g: " << diffuse_component.Green << "   b: " << diffuse_component.Blue << std::endl;

    col.Add(specular_component);
}

void AshikhminMaterial::GenerateSample(const Intersection &isect,
        const glm::vec3 &inDir,
        glm::vec3 &outDir,
        Color &outColor)
{
    float pi=3.1415926f;
    glm::vec3 utan = glm::normalize(isect.TangentU);
    glm::vec3 vtan = glm::normalize(isect.TangentV);
    glm::vec3 normal = glm::normalize(isect.Normal);

    bool do_diffuse;
    float dors = ((float) rand()) / RAND_MAX;
    if (dors < rd / (rd + rs)) do_diffuse = true;
    else                       do_diffuse = false;

    if (do_diffuse) {
        glm::vec3 normal = isect.Normal;

        // Cosine-weighted hemisphere.
        float s = ((float) rand()) / RAND_MAX;
        float t = ((float) rand()) / RAND_MAX;
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
        outColor = Color::BLACK;
        outColor.AddScaled(DiffuseColor, rd);
        // outColor.Scale(rd / (rd+rs));
    } else {
        // Random nums
        float e1 = ((float) rand()) / RAND_MAX;
        float e2 = ((float) rand()) / RAND_MAX;
        float e3 = ((float) rand()) / RAND_MAX;

        float phi = glm::atan( glm::sqrt( (nu + 1) / (nv + 1) ) * glm::tan(e1 * pi / 2.f) );
        if      (e3 < 0.25f)    phi = phi; // do nothing, first quadrant
        else if (e3 < 0.5f)     phi = pi - phi; // flip around pi/2
        else if (e3 < 0.75f)    phi = 1.5f * pi - phi;
        else                    phi = 2.f * pi - phi;
        float denom = (nu * glm::pow(glm::cos(phi), 2)) + (nv * glm::pow(glm::sin(phi), 2))  + 1;
        float theta = glm::acos( glm::pow(1.f-e2, 1.f / denom ));

        // phi and theta to cartesian coordinates
        glm::vec3 i = (glm::cos(phi) * glm::sin(theta)) * utan;
        glm::vec3 j = (glm::sin(phi) * glm::sin(theta)) * vtan;
        glm::vec3 k = glm::cos(theta) * normal;
        glm::vec3 h = glm::normalize(i + j + k);

        // Get k2 from half-vector
        glm::vec3 k2 = glm::normalize(inDir + 2 * glm::dot(-inDir, h) * h);

        // Set out direction and out color.
        outDir = k2;

        outColor = Color::BLACK;
        outColor.AddScaled(SpecularColor, rs);
        // outColor.Scale(rs / (rd+rs));

        /*
        // Flip to choose either diffuse or specular sample.
        float e4 = ((float) rand()) / RAND_MAX;
        if (e4 < rd / (rd + rs)) outColor = DiffuseColor;
        else                     outColor = SpecularColor;
        */
    }
}

void AshikhminMaterial::SetDiffuseColor(Color c) {
    DiffuseColor = c;
}

void AshikhminMaterial::SetSpecularColor(Color c) {
    SpecularColor = c;
}

void AshikhminMaterial::SetDiffuseLevel(float level) {
    rd = level;
}

void AshikhminMaterial::SetSpecularLevel(float level) {
    rs = level;
}

void AshikhminMaterial::SetRoughness(float nu, float nv) {
    this->nu = nu;
    this->nv = nv;
}
