
#include "imgui.h"
#include "Renderer.hpp"
#include "raytracing/core/Ray.hpp"
#include "raytracing/core/HitPayload.hpp"
#include "raytracing/core/Light.hpp"
#include <stdio.h>
#include <omp.h>

#define QUARTER_RENDER 1

Raytracing::Renderer::Renderer()
    : camera({0, 0, 2}, {0, 0, 0}, {0, 1, 0}, 15 * g_pi / 16, 0, 500)
{
    image = new ImageWrapper();
    scene = Scene();
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

// render every pixel of the screen.
void Raytracing::Renderer::Render(const Scene &renderedScene, const Camera &renderingCamera)
{
    scene = renderedScene;
    const Light light = {1.f, {1.f, 1.f, 1.f}};
    camera = renderingCamera;
    size_t x, y;
    const std::vector<glm::vec3> dirs = camera.getRayDirections();
    #pragma omp parallel for private(x) schedule(dynamic)
    for (y = 0; y < getHeight(); y++)
    {
        for (x = 0; x < getWidth(); x++)
        {
            const uint32_t pixelIndex = x + y * getWidth();
            const int bounces = 2;
            Ray ray;
            ray.origin = camera.getPosition();
            ray.direction = dirs[pixelIndex];
            glm::vec3 col(0.f);
            for (int bounce = 0; bounce < bounces; bounce++)
            {
                HitPayload payload = traceRay(&ray);
                if (payload.hitDistance < 0)
                {
                    // we missed all spheres
                    break;
                }
                ray.origin = payload.worldPosition + EPSILON * payload.worldNormal;
                ray.direction = glm::reflect(ray.direction, payload.worldNormal);
                const float lightIntensity = glm::max(glm::dot(glm::normalize(light.direction), payload.worldNormal), 0.f);
                const Sphere& sphere = scene.getListSphere()[payload.objectIndex];
                col += lightIntensity * sphere.reflection;
            }
            image->setPixel(pixelIndex, IM_COL32(col.r, col.g, col.b, 255));
        }
    }

    image->update();
}

Raytracing::HitPayload Raytracing::Renderer::traceRay(Ray *ray)
{
    // (o + t*d - p) . (o + t*d - p) - r*r = 0
    // <=> (d . d) * t^2 + 2 * d . (o - p) * t + (o - p) . (o - p) - r*r = 0
    //
    // Discriminant check
    // (2 * d . (o - p))^2 - 4 * (d . d) * ((o - p) . (o - p) - r*r) <? 0
    // <=> (d . (o - p))^2 - (d . d) * ((o - p) . (o - p) - r*r) <? 0
    // <=> (d . op)^2 - 1 * (op . op - r*r) <? 0
    // <=> b^2 - (op . op) + r*r <? 0
    // <=> D <? 0
    //
    // Solutions
    // t = (- 2 * d . (o - p) +- 2 * sqrt(D)) / (2 * (d . d))
    // <=> t = dop +- sqrt(D)
    int index = -1;
    const std::vector<Sphere> list = scene.getListSphere();
    double hitDistance = camera.getFar();

    for (size_t i = 0; i < list.size(); i++)
    {
        const Sphere &sphere = list[i];

        const glm::vec3 op = sphere.center - ray->origin;
        const double dop = glm::dot(ray->direction, op);
        const double D = dop * dop - glm::dot(op, op) + sphere.radius * sphere.radius;

        if (D < 0.0 || dop < 0)
        {
            continue;
        }

        const double sqrtD = sqrt(D);

        const double tmin = dop - sqrtD;
        // check if the point is in the clip region and before the last hit
        if (camera.getNear() < tmin && tmin < hitDistance)
        {
            hitDistance = tmin;
            index = i;
        }

        const double tmax = dop + sqrtD;
        if (camera.getNear() < tmax && tmax < hitDistance)
        {
            hitDistance = tmax;
            index = i;
        }
    }
    if (index == -1)
        return miss(ray);
    return closestHit(ray, hitDistance, index);
}

Raytracing::HitPayload Raytracing::Renderer::closestHit(Ray *ray, float hitDistance, int objectIndex)
{
    // returned struct
    HitPayload payload;
    payload.hitDistance = hitDistance;

    // get the hit sphere
    const std::vector<Sphere> spheres = scene.getListSphere();
    payload.objectIndex = objectIndex;

    // compute the hit position
    payload.worldPosition = ray->origin + hitDistance * ray->direction;
    payload.worldNormal = glm::normalize(payload.worldPosition - spheres[payload.objectIndex].center);
    // if (payload.objectIndex <= 0)
    // {
    //     std ::cout << "------------------------------------------------" << std::endl;
    //     std ::cout << "p.hitDist = " << payload.hitDistance << std::endl;
    //     std ::cout << "p.pos = " << payload.worldPosition.x << " " << payload.worldPosition.y << " " << payload.worldPosition.z << std::endl;
    //     std ::cout << "p.normal = " << payload.worldNormal.x << " " << payload.worldNormal.y << " " << payload.worldNormal.z << std::endl;
    //     std ::cout << "------------------------------------------------" << std::endl;
    // }
    return payload;
}

Raytracing::HitPayload Raytracing::Renderer::miss(Ray *ray)
{
    Raytracing::HitPayload payload;
    payload.hitDistance = -1;
    return payload;
}
