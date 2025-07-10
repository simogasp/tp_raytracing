
#include "imgui.h"
#include "Renderer.hpp"
#include "raytracing/core/Ray.hpp"
#include <stdio.h>
# include <omp.h>

# define QUARTER_RENDER 1

Raytracing::Renderer::Renderer()
    : camera({0, 0, 2}, {0, 0, 0}, {0, 1, 0}, 15 * g_pi / 16, 0, 500)
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

void Raytracing::Renderer::onResize(const uint32_t newWidth, const uint32_t newHeight)
{
    if (image && image->getWidth() == newWidth && image->getHeight() == newHeight)
        return;
    
    image->resize(newWidth, newHeight);
}

bool cond(int x, int y, int w, int h) {
    return (x % (w / 4) < 1 && y % (h / 4) < 1);
}

// render every pixel of the screen.
void Raytracing::Renderer::Render(const Scene& renderedScene, const Camera& renderingCamera)
{
    scene = renderedScene;
    camera = renderingCamera;
    size_t x, y;
    Ray ray;
    ray.setOrigin(camera.getPosition());
    ray.setFar(camera.getFar());
    ray.setNear(camera.getNear());
    const std::vector<glm::vec3> dirs = camera.getRayDirections();
    // #pragma omp parallel for collapse(2)
    for (y = 0; y < getHeight(); y++)
    {
        for (x = 0; x < getWidth(); x++)
        {
            ray.setDirection(dirs[x + y * getWidth()]);
            ray.reset();
            const ImColor pixelColor = traceRay(&ray);
            image->setPixel(x + y * getWidth(), pixelColor);
        }
    }

    image->update();
}


uint32_t Raytracing::Renderer::traceRay(Ray *ray)
{
    // img (green, yellow,
    //      black, red)
    // return IM_COL32((float) x / getWidth() * 255, (1 - (float) y / getHeight()) * 255, 0, 255);

    // special points (printed for set direction)
    // if (cond(x, y, getWidth(), getHeight()))
    // {
    //     return 0xFF00FFFF;
    // }
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
    // const float alpha = (1 - abs(ray->getDirection().m_y)) * 255;
    // if (ray->getDirection().m_y > 0) {
    //     return IM_COL32(184, 115, 51, 255);
    // }
    // printf("m_y = %f %f %f %f\n", ray.getDirection().m_x, ray.getDirection().m_y, ray.getDirection().m_z, alpha);
    // return IM_COL32(alpha, alpha, 255, 255);
    return IM_COL32(0, 0, 0, 255);
}
