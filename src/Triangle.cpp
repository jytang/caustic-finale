#include "Triangle.h"

Triangle::Triangle() {}

bool Triangle::Intersect(const Ray &ray, Intersection &hit) const
{
    glm::vec3 negd = -ray.Direction;
    glm::vec3 bmina = Vtx[1]->Position-Vtx[0]->Position;
    glm::vec3 cmina = Vtx[2]->Position-Vtx[0]->Position;
    glm::vec3 pmina = ray.Origin-Vtx[0]->Position;
    glm::vec3 cross1 = glm::cross(bmina, cmina);
    glm::vec3 cross2 = glm::cross(pmina, cmina);
    glm::vec3 cross3 = glm::cross(bmina, pmina);

    // for smooth shading
    glm::vec3 cminb = Vtx[2]->Position-Vtx[1]->Position;
    glm::vec3 aminb = Vtx[0]->Position-Vtx[1]->Position;
    glm::vec3 aminc = Vtx[0]->Position-Vtx[2]->Position;
    glm::vec3 bminc = Vtx[1]->Position-Vtx[2]->Position;
    glm::vec3 bnorm = glm::normalize(glm::cross(cminb, aminb));
    glm::vec3 cnorm = glm::normalize(glm::cross(aminc, bminc));

    float det = glm::determinant(glm::mat3(negd.x,negd.y,negd.z,
                                           bmina.x,bmina.y,bmina.z,
                                           cmina.x,cmina.y,cmina.z));
    float t = glm::determinant(glm::mat3(pmina.x,pmina.y,pmina.z,
                                           bmina.x,bmina.y,bmina.z,
                                           cmina.x,cmina.y,cmina.z)) / det;
    float a = glm::determinant(glm::mat3(negd.x,negd.y,negd.z,
                                           pmina.x,pmina.y,pmina.z,
                                           cmina.x,cmina.y,cmina.z)) / det;
    float b = glm::determinant(glm::mat3(negd.x,negd.y,negd.z,
                                           bmina.x,bmina.y,bmina.z,
                                           pmina.x,pmina.y,pmina.z)) / det;

    if (a > 0 && b > 0 && a + b < 1 && t > 0.00001f)
    {
        if (t < hit.HitDistance)
        {
            hit.Position = ray.Origin + t * ray.Direction;
            hit.Normal = (1-a-b) * glm::normalize(cross1);
            hit.Normal += a * bnorm;
            hit.Normal += b * cnorm;
            hit.Normal = glm::normalize(hit.Normal);
            // NORMAL FLIP IF NORMAL DOES NOT POINT TOWARDS RAY
            if (glm::dot(hit.Normal,ray.Origin-hit.Position) < 0)
                hit.Normal = -hit.Normal;
            // Compute tangents
            hit.TangentU=glm::cross(glm::vec3(0,1,0),hit.Normal);
            if(glm::length(hit.TangentU)<0.0001)
                hit.TangentU=glm::cross(glm::vec3(1,0,0),hit.Normal);
            hit.TangentU=glm::normalize(hit.TangentU);
            hit.TangentV=glm::cross(hit.Normal,hit.TangentU);

            hit.HitDistance = t;
            hit.Mtl = Mtl;
            return true;
        }
    }
    return false;
}

Vertex &Triangle::GetVtx(int i) const
{
    return *Vtx[i];
}
