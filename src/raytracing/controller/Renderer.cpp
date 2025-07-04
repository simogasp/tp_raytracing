
#include "imgui.h"
#include "Renderer.hpp"
#include "raytracing/core/Ray.hpp"
#include <stdio.h>
# include <omp.h>

Raytracing::Renderer::Renderer()
    : camera({0, +0.25, 2}, {0, 0, 0}, {0, 1, 0}, 10, 15 * g_pi / 16, 10, 500)
{
    image = new ImageWrapper();
    scene = Scene();
    scene.addRandomSphere();
}

Raytracing::Renderer::~Renderer()
{
    free(image);
}

uint32_t Raytracing::Renderer::getWidth()
{
    return image->getWidth();
}

uint32_t Raytracing::Renderer::getHeight()
{
    return image->getHeight();
}

GLuint Raytracing::Renderer::getTextureId()
{
    return image->getTextureId();
}

void Raytracing::Renderer::OnResize(const uint32_t newWidth, const uint32_t newHeight)
{
    if (image && image->getWidth() == newWidth && image->getHeight() == newHeight)
        return;
    image->resize(newWidth, newHeight);
    delete[] rays;
    const uint32_t length = newWidth * newHeight;
    rays = new Ray[length];
    setRayDirection();
}

// render every pixel of the screen.
void Raytracing::Renderer::Render()
{
    size_t x, y;
#pragma omp parallel for collapse(2) private(x, y)
    for (y = 0; y < getHeight(); y++)
    {
        for (x = 0; x < getWidth(); x++)
        {
            image->setPixel(x + y * getWidth(), perPixel(x, y));
        }
    }

    image->update();
}

void Raytracing::Renderer::setRayDirection()
{
    const float screenRatio = (float) getWidth() / getHeight();
    uint32_t x, y;
    Vector3 lookAtVector;
    #pragma omp parallel for collapse(2) private(x, y, lookAtVector)
    for (y = 0; y < getHeight(); y++)
    {
        for (x = 0; x < getWidth(); x++)
        {
            image->setPixel(x + y * getWidth(), perPixel(x, y));

            
            
            // relative placement on the screen of the pixel
            const float relativeX = 2 * (float) x / (getWidth() - 1) - 1;
            const float relativeY = 2 * (float) y / (getHeight() - 1) - 1;
            // const float lx = screenDistance * tan(camera.getFieldOfView() / 2);
            // const float ly = screenDistance * tan(screenRatio * camera.getFieldOfView() / 2);

            
            // // rotation angle
            // const float thetax = camera.getFieldOfView() / 2 * relativeX;
            // const float thetay = camera.getFieldOfView() / 2 * relativeY * screenRatio;

            // const float sinthetax = sin(thetax);
            // const float costhetax = sin(thetax);

            // const float sinthetay = sin(thetay);
            // const float costhetay = sin(thetay);

            // point on the screen is also
            // const Vector3 point = Vector3(
            //     costhetax * relativeX + sinthetax * screenDistance,
            //     sinthetax * sinthetay * x + costhetay * y + sinthetax * screenDistance,
            //     costhetay * x + sinthetay * y + costhetax * costhetay * screenDistance);

            lookAtVector = camera.getLookAt() - camera.getPosition();
            lookAtVector = Normalize(lookAtVector);
            const Vector3 point = lookAtVector + Vector3(
                relativeX,
                relativeY,
                0
            );

            // create a Ray from the camera to the point of the screen
            const Vector3 dir = point - camera.getPosition();
            const Vector3 res = Normalize(dir);
            Ray *ray = &rays[x + y * getWidth()];
            ray->setDirection(res);
            ray->setFar(camera.getFar());
            ray->setNear(camera.getNear());
            ray->reset();
            if ((x % 150 == 0 && y % 150 == 0) || (x == getWidth() - 1 && y == getHeight() - 1))
            {
                // printf("tx = %f ty = %f dir = %lf %lf %lf\n", thetax, thetay, dir.m_x, dir.m_y, dir.m_z);
                printf(
                    "dir at (%d, %d) (%f, %f) = (%lf, %lf, %lf) sr = %f\n",
                    x,
                    y, 
                    relativeX, 
                    relativeY, 
                    rays[x + y * getWidth()].getDirection().m_x, 
                    rays[x + y * getWidth()].getDirection().m_y, 
                    rays[x + y * getWidth()].getDirection().m_z, 
                    screenRatio
                );
            }
        }
    }
}

uint32_t Raytracing::Renderer::perPixel(const uint32_t x, const uint32_t y)
{
    // img (green, yellow,
    //      black, red)
    // return IM_COL32((float) x / getWidth() * 255, (1 - (float) y / getHeight()) * 255, 0, 255);

    Ray ray = rays[x + y * getWidth()];
    for (size_t i = 0; i < scene.getListSphere().size(); i++)
    {
        if (scene.getListSphere()[i].intersect(ray))
        {
            printf("hits on (%d, %d)\n", x, y);
            return IM_COL32(255, 0, 255, 255); // magenta
        }
    }

    // special points (printed for set direction)
    if ((x % 150 <= 2 && y % 150 <= 2) || (x >= getWidth() - 2 && y >= getHeight() - 2))
    {
        return 0xFF000000;
    }
    // printf("dir = %f\n", ray.getDirection().m_y);
    if (ray.getDirection().m_y > 0) {
        return IM_COL32(184, 115, 51, 255);
    }
    const float alpha = ray.getDirection().m_y * 255 + 255;
    // printf("m_y = %f %f %f %f\n", ray.getDirection().m_x, ray.getDirection().m_y, ray.getDirection().m_z, alpha);
    return IM_COL32(alpha, alpha, 255, 255);
}
