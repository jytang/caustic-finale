#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Scene.h"
#include "Bitmap.h"
#include "ray_trace.h"

class Camera
{
public:
    Camera(RayTrace &rt);
	void SetResolution(int width, int height);
	void SetFOV(float fov);
	void SetAspect(float aspect);

	void LookAt(const glm::vec3& pos, const glm::vec3& at, const glm::vec3& up);
	void Render();
	void SaveBitmap(const std::string &fname);
    void SetSuperSample(int xsamples, int ysamples); // supersample each pixel in a x*y grid
    void SetJitter(bool enable); // enable/disable jittering of each supersample
    void SetShirley(bool enable); // enable/disable Shirley mapping
private:
    bool EnableJitter, EnableShirley;
    int x_samples, y_samples;
	int width = 0, height = 0;
	float fov = 0, aspect = 0, hfov = 0;
    Bitmap *bitmap = NULL;
    glm::mat4 matrix;
    glm::vec3 a,b,c,d;
    float scaleX,scaleY;
    Ray ray;
    RayTrace &rt;

	void RenderPixel(int x, int y);
    bool InShadow(glm::vec3 pos);
};
