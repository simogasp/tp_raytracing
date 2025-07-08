
#include "imgui.h"
#include "Renderer.hpp"
#include "raytracing/core/Ray.hpp"
#include <stdio.h>
# include <omp.h>

# define QUARTER_RENDER 0

Raytracing::Renderer::Renderer()
    : camera({0, 0, 2}, {0, 0, 0}, {0, 1, 0}, 10, 15 * g_pi / 16, 0, 500)
{
    image = new ImageWrapper();
    scene = Scene();
    scene.addRandomSphere();
    pixel = 0;
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

bool cond(int x, int y, int w, int h) {
    return (x % (w / 4) < 1 && y % (h / 4) < 1);
}

// render every pixel of the screen.
void Raytracing::Renderer::Render()
{
    # if QUARTER_RENDER
    pixel = ++pixel % 4;
    size_t x, y;
    #pragma omp parallel for collapse(2)
    for (y = pixel % 2; y < getHeight(); y+=2)
    {
        for (x = pixel < 2 ? 0 : 1; x < getWidth(); x+=2)
        {
            rays[x + y * getWidth()].reset();
            const ImColor pixelColor = perPixel(x, y);
            image->setPixel(x + y * getWidth(), pixelColor);
            image->setPixel(x + y * getWidth(), pixelColor);
            image->setPixel(x + y * getWidth(), pixelColor);
            image->setPixel(x + y * getWidth(), pixelColor);
        }
    }
    # else
    size_t x, y;
    #pragma omp parallel for collapse(2)
    for (y = 0; y < getHeight(); y++)
    {
        for (x = 0; x < getWidth(); x++)
        {
            rays[x + y * getWidth()].reset();
            image->setPixel(x + y * getWidth(), perPixel(x, y));
        }
    }
    # endif

    image->update();
}

void Raytracing::Renderer::setFov(const double newValue)
{
    camera.setFov(newValue);
    setRayDirection();
}

void Raytracing::Renderer::cameraForward()
{
    camera.forward();
}

void Raytracing::Renderer::cameraBackward()
{
    camera.backward();
}

void Raytracing::Renderer::cameraLeftShift()
{
    camera.left();
}

void Raytracing::Renderer::cameraRightShift()
{
    camera.right();
}

void Raytracing::Renderer::cameraUpShift()
{
    camera.up();
}

void Raytracing::Renderer::cameraDownShift()
{
    camera.down();
}

void Raytracing::Renderer::cameraLookLeft()
{
    camera.lookLeft();
}

void Raytracing::Renderer::cameraLookRight()
{
    camera.lookRight();
}

void Raytracing::Renderer::cameraRotateAntiClockWise()
{
    camera.rotateAntiClockWise();
}

void Raytracing::Renderer::cameraRotateClockWise()
{
    camera.rotateClockWise();
}

void Raytracing::Renderer::updateRay()
{
    setRayDirection();
}

void Raytracing::Renderer::setRayDirection()
{
    const float invScreenRatio = (float) getHeight() / getWidth();
    uint32_t x, y;
    Vector3 lookAtVector = camera.getLookAt() - camera.getPosition();
    // #pragma omp parallel for collapse(2)
    for (y = 0; y < getHeight(); y++)
    {
        for (x = 0; x < getWidth(); x++)
        {
            image->setPixel(x + y * getWidth(), perPixel(x, y));
            
            
            
            // relative placement on the screen of the pixel
            const float relativeX = 2 * (float) x / (getWidth() - 1) - 1;
            const float relativeY = 2 * (float) y / (getHeight() - 1) - 1;

            
            // rotation angle
            const float theta = camera.getFieldOfView() / 2 * relativeX;
            const float phi = camera.getFieldOfView() / 2 * relativeY * invScreenRatio;

            const float sintheta = sin(theta);
            const float costheta = cos(theta);

            const float sinphi = sin(phi);
            const float cosphi = cos(phi);

            // point on the screen is also

            const Vector3 dir = camera.baseChangment(Vector3(
                cosphi * sintheta, // y nor
                sinphi, // z nor
                cosphi * costheta // x nor
            ));

            // create a Ray from the camera to the point of the screen
            Ray *ray = &rays[x + y * getWidth()];
            ray->setOrigin(camera.getPosition());
            ray->setDirection(dir);
            ray->setFar(camera.getFar());
            ray->setNear(camera.getNear());
            ray->reset();
            // if (cond(x, y, getWidth(), getHeight()))
            // {
            //     printf("phi = %lf° theta = %lf° costheta = %lf sinteta = %lf cos pi = %lf sin pi = %lf\n", phi * 180 / g_pi, theta * 180 / g_pi,  costheta, sintheta, cosphi, sinphi);
            //     // printf("tx = %f ty = %f dir = %lf %lf %lf\n", thetax, thetay, dir.m_x, dir.m_y, dir.m_z);
            //     printf(
            //         "dir at (%d, %d) (%f, %f) = (%lf, %lf, %lf) (%lf, %lf, %lf) (%lf, %lf, %lf)\n",
            //         x,
            //         y, 
            //         relativeX,
            //         relativeY,
            //         rays[x + y * getWidth()].getDirection().m_x,
            //         rays[x + y * getWidth()].getDirection().m_y,
            //         rays[x + y * getWidth()].getDirection().m_z, 
            //         lookAtVector.m_x,
            //         lookAtVector.m_y,
            //         lookAtVector.m_z,
            //         dir.m_x,
            //         dir.m_y,
            //         dir.m_z
            //     );
            // }
        }
    }
}

uint32_t Raytracing::Renderer::perPixel(const uint32_t x, const uint32_t y)
{
    // img (green, yellow,
    //      black, red)
    // return IM_COL32((float) x / getWidth() * 255, (1 - (float) y / getHeight()) * 255, 0, 255);

    // special points (printed for set direction)
    // if (cond(x, y, getWidth(), getHeight()))
    // {
    //     return 0xFF00FFFF;
    // }
    Ray *ray = &rays[x + y * getWidth()];
    for (size_t i = 0; i < scene.getListSphere().size(); i++)
    {
        if (scene.getListSphere()[i].intersect(ray))
        {
            return ray->getColor();
        }
    }

    // if (x == 36 || y == 134) {
    //     return 0xFF00FFFF;
    // }

    // printf("dir = %f\n", ray.getDirection().m_y);
    // return IM_COL32((1 + ray->getDirection().m_y) / 2 * 255,(1 + ray->getDirection().m_y) / 2 * 255, (1 + ray->getDirection().m_y) / 2 * 255, 255);
    // return IM_COL32((1 + ray->getDirection().m_x) / 2 * 255, (1 + ray->getDirection().m_y) / 2 * 255, (1 + ray->getDirection().m_z) / 2 * 255, 255);
    const float alpha = (1 - abs(ray->getDirection().m_y)) * 255;
    if (ray->getDirection().m_y > 0) {
        return IM_COL32(184, 115, 51, 255);
    }
    // printf("m_y = %f %f %f %f\n", ray.getDirection().m_x, ray.getDirection().m_y, ray.getDirection().m_z, alpha);
    return IM_COL32(alpha, alpha, 255, 255);
}
