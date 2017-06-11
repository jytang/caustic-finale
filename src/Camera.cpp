#include "Camera.h"
#include "Material.h"

#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>

Camera::Camera(RayTrace &rt) : rt(rt) {}

void Camera::SetResolution(int width, int height)
{
    this->width = width;
    this->height = height;

    if (bitmap != NULL)
        delete bitmap;

    bitmap = new Bitmap(width, height);
}

void Camera::SetFOV(float fov)
{
    this->fov = glm::radians(fov);

    if (aspect != 0)
        hfov = 2.f * glm::atan( aspect * glm::tan( this->fov / 2.f ) );
}

void Camera::SetAspect(float aspect)
{
    this->aspect = aspect;

    if (fov != 0 && hfov == 0)
        hfov = 2.f * glm::atan( aspect * glm::tan( this->fov / 2.f ) );
}


void Camera::LookAt(const glm::vec3& pos, const glm::vec3& at, const glm::vec3& up)
{
    glm::vec3 d = pos;
    glm::vec3 c = glm::normalize(d - at);
    glm::vec3 a = glm::normalize(glm::cross(up,c));
    glm::vec3 b = glm::cross(c,a);

    matrix = glm::mat4(a.x,a.y,a.z,0,b.x,b.y,b.z,0,c.x,c.y,c.z,0,d.x,d.y,d.z,1);
}

void Camera::Render()
{
    // seed rng
    srand(time(NULL));

    // START TIMER.
    std::clock_t start;
    double duration;
    start = std::clock();
    std::cout << "Starting render..." << std::endl;

    // Precalculate things
    a = glm::vec3(matrix[0]);
    b = glm::vec3(matrix[1]);
    c = glm::vec3(matrix[2]);
    d = glm::vec3(matrix[3]);
    scaleX = 2.f * glm::tan( hfov / 2.f );
    scaleY = 2.f * glm::tan( fov / 2.f );

    int x, y;
    float percent = 0;
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            if (x*y > percent * height * width) {
                std::cerr << percent * 200 << "%..";
                percent += 0.01f;
            }
            RenderPixel(x, y);
        }
    }

    // REPORT TIMER.
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << "Render duration: " << duration << std::endl;
}

void Camera::SaveBitmap(const std::string &fname)
{
    bitmap->SaveBMP(fname.c_str());
}

void Camera::RenderPixel(int x, int y)
{
    Color final_color(0.f, 0.f, 0.f);
    float x_step = (1.f / x_samples);
    float half_x_step = x_step /2.f;
    float y_step = (1.f / y_samples);
    float half_y_step = y_step /2.f;
    for (int i = 0; i < x_samples; ++i) {
        for (int j = 0; j < y_samples; ++j) {
            // uniform supersamples
            float sub_x = i * x_step + half_x_step;
            float sub_y = j * y_step + half_y_step;
            if (EnableJitter) {
                sub_x += (((float) rand()) / RAND_MAX) * x_step - half_x_step;
                sub_y += (((float) rand()) / RAND_MAX) * y_step - half_y_step;
            }
            if (EnableShirley) {
                if (sub_x < 0.5f) sub_x = -0.5f + glm::sqrt(2 * sub_x);
                else              sub_x = 1.5f - glm::sqrt(2 - 2 * sub_x);
                if (sub_y < 0.5f) sub_y = -0.5f + glm::sqrt(2 * sub_y);
                else              sub_y = 1.5f - glm::sqrt(2 - 2 * sub_y);
            }

            // Get supersample of pixel, range from -0.5f to 0.5f
            float fx = (float(x) + sub_x) / float(width) - 0.5f;
            float fy = (float(y) + sub_y) / float(height) - 0.5f;

            // build ray and trace it
            ray.Origin = d;
            ray.Direction = glm::normalize(fx*scaleX*a + fy*scaleY*b - c);
            Intersection hit;
            rt.TraceRay(ray, hit);

            // add to final color, to be averaged out later
            final_color.Add(hit.Shade);
        }
    }
    // average the final color
    final_color.Scale(1.f / (x_samples * y_samples));
    bitmap->SetPixel(x,y,final_color.ToInt());
}

void Camera::SetSuperSample(int xsamples, int ysamples)
{
    x_samples = xsamples;
    y_samples = ysamples;
}

void Camera::SetJitter(bool enable)
{
    EnableJitter = enable;
}

void Camera::SetShirley(bool enable)
{
    EnableShirley = enable;
}
