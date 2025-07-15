
#include "imgui.h"
#include "Renderer.hpp"
#include "raytracing/core/Ray.hpp"
#include "raytracing/core/HitPayload.hpp"
#include "raytracing/core/Light.hpp"
#include <stdio.h>
#include <omp.h>

// number of bounce to made
# define BOUNCES 2

Raytracing::Renderer::Renderer()
    : camera({0, 0, 2}, {0, 0, 0}, {0, 1, 0}, 15 * g_pi / 16, 0, 500)
{
    image = new ImageWrapper();
    scene = Scene();
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

    // resize GPU image
    delete[] imageData;
    imageData = nullptr;
    delete[] accumulatedData;
    accumulatedData = nullptr;
    // update dims
    imageData = new uint32_t[getWidth() * getHeight()];
    accumulatedData = new glm::vec4[getWidth() * getHeight()];

    image->setData(imageData);

}

// render every pixel of the screen.
void Raytracing::Renderer::Render(const Scene &renderedScene, const Camera &renderingCamera)
{
    FrameId++;
    if (FrameId /*% 100*/ == 1)
    {
        memset(accumulatedData, 0, getWidth() * getHeight() * sizeof(*accumulatedData));
        FrameId = 1;
        std::cout << "reset" << std::endl;
    }
    
    scene = renderedScene;
    camera = renderingCamera;
    size_t x, y;
    const std::vector<glm::vec3> dirs = camera.getRayDirections();
    // #pragma omp parallel for private(x) schedule(dynamic)
    for (y = 0; y < getHeight(); y++)
    {
        for (x = 0; x < getWidth(); x++)
        {
            // helper for the pixel index
            const uint32_t pixelIndex = x + y * getWidth();

            // ray construction
            Ray ray;
            ray.origin = camera.getPosition();
            ray.direction = dirs[pixelIndex];
            ray.bounce = 0;

            // final color by substraction method
            glm::vec3 light(0.f);
            glm::vec3 colorContribution(1.f);
            float shiny = 1.f;


            // iteration on bounce
            for (; ray.bounce < BOUNCES; ray.bounce++)
            {
                HitPayload payload = traceRay(&ray);

                if (payload.hitDistance < 0)
                {
                    // we missed all spheres
                    const double rgColor = (1 - abs(ray.direction.y));
                    const glm::vec3 skyColor(rgColor, rgColor, 1);
                    // std::cout << skyColor.r << " " << skyColor.g << " " << skyColor.b << std :: endl;
                    light += skyColor;
                    break;
                }

                // update the color
                const Sphere sphere = scene.getListSphere()[payload.objectIndex];
                const Material mat = scene.getListMaterial()[sphere.materialIndex];
                // std::cout<<"mat = "<<mat.reflection.r << " " << mat.reflection.g << " " << mat.reflection.b << std::endl;
                // std::cout << (shiny * mat.reflection).r << " " << (shiny * mat.reflection).g << " "<< (shiny * mat.reflection).b << " " << std::endl;
                colorContribution *= shiny * mat.reflection;
                // std::cout<<"col"<<colorContribution.r << " " << colorContribution.g << " " << colorContribution.b << std::endl;
                shiny *= mat.shinyness;
                light += mat.emission;
                
                // update the ray
                ray.origin = payload.worldPosition + EPSILON * payload.worldNormal;
                // diffuse ray
                ray.direction = /* glm::reflect(ray.direction, payload.worldNormal) */ payload.worldNormal + (float) mat.roughness * glm::normalize(glm::vec3((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX));
            }
            const glm::vec4 frameColor = glm::vec4(
                (int) (light.r * colorContribution.r * 255),
                (int) (light.g * colorContribution.g * 255), 
                (int) (light.b * colorContribution.b * 255),
                255
            );

            // draw the pixel
            accumulatedData[pixelIndex] += frameColor;
            const ImColor outColor = IM_COL32(
                accumulatedData[pixelIndex].r / (float) FrameId, 
                accumulatedData[pixelIndex].g / (float) FrameId, 
                accumulatedData[pixelIndex].b / (float) FrameId, 
                accumulatedData[pixelIndex].a / (float) FrameId
            );
            const ImColor frameCol = IM_COL32(frameColor.r, frameColor.g, frameColor.b, frameColor.a);
            imageData[pixelIndex] = outColor;
        }
    }

    image->setData(imageData);
}

void Raytracing::Renderer::resetAcc()
{
    FrameId = 0;
}

Raytracing::HitPayload Raytracing::Renderer::traceRay(Ray *ray)
{
    
    int index = -1;
    const std::vector<Sphere> list = scene.getListSphere();
    double hitDistance = camera.getFar();

    for (size_t i = 0; i < list.size(); i++)
    {
        const Sphere &sphere = list[i];
        const double sphereHitDistance = sphere.intersect(&camera, ray);

        if (sphereHitDistance > 0 && sphereHitDistance < hitDistance)
        {
            hitDistance = sphereHitDistance;
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
