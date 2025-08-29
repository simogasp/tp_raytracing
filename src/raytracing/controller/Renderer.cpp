
#include "imgui.h"
#include "Renderer.hpp"
#include "raytracing/core/Ray.hpp"
#include "raytracing/core/HitPayload.hpp"
# include <glm/gtc/constants.hpp>

// number of bounce to made
#define BOUNCES 2

// devide the resolution by 4 to accelerate the computation
#define RESON4 0

Raytracing::Renderer::Renderer()
    : camera({0, 0, 2}, {0, 0, 0}, {0, 1, 0}, 15 * glm::pi<float>() / 16, 0, 500)
{
    image = new ImageWrapper();
    attenuationFormula = 1;
}

uint32_t Raytracing::Renderer::getWidth() const
{
    return image->getWidth();
}

uint32_t Raytracing::Renderer::getHeight() const
{
    return image->getHeight();
}

uint32_t Raytracing::Renderer::getFrameId() const 
{ 
    return frameId;
}

GLuint Raytracing::Renderer::getTextureId() const
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
    accumulatedData = new glm::vec3[getWidth() * getHeight()];
    frameId = 0;
    image->setData(imageData);
}

void Raytracing::Renderer::Render(const Scene &renderedScene, const Camera &renderingCamera)
{
    // render every pixel of the screen.

    frameId++;
    // reset the accumulator
    if (frameId == 1)
    {
        memset(accumulatedData, 0, getWidth() * getHeight() * sizeof(*accumulatedData));
    }

    // store the given scene and camera
    scene = renderedScene;
    camera = renderingCamera;

    // get the computed ray direction from camera
    const std::vector<glm::vec3> dirs = camera.getRayDirections();
#pragma omp parallel for
#if RESON4
    for (size_t y = 0; y < getHeight(); y += 2)
#else
    for (size_t y = 0; y < getHeight(); y++)
#endif
    {
#if RESON4
        for (size_t x = 0; x < getWidth(); x += 2)
#else
        for (size_t x = 0; x < getWidth(); x++)
#endif
        {
            // helper for the pixel index
            const uint32_t pixelIndex = (uint32_t) (x + y * getWidth());
            
            //++ // TODO : handle the display of each pixel
            
            //<!!
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
            for (; ray.bounce <= BOUNCES; ray.bounce++)
            {
                HitPayload payload = traceRay(&ray);

                if (payload.hitDistance < 0)
                {
                    // we missed all spheres
                    const double rgColor = (1 - abs(ray.direction.y)) / 2 + 0.3;
                    const glm::vec3 skyColor(rgColor, rgColor, 1);
                    light += skyColor;
                    break;
                }

                // update the color
                const Sphere sphere = scene.getListSphere()[payload.objectIndex];
                const Material mat = scene.getListMaterial()[sphere.materialIndex];
                colorContribution = (1.f - shiny) * colorContribution + shiny * mat.reflection;
                shiny *= mat.shinyness;

                if (ray.bounce > 0)
                    light += getAttenuation(payload, mat) * mat.getEmission();
                else
                    light += mat.getEmission();

                // update the ray200

                // noise around normal
                const glm::vec3 noiseN = glm::normalize(glm::vec3(
                    2.0 * ((float)rand() / (float)RAND_MAX) - 1.0,
                    2.0 * ((float)rand() / (float)RAND_MAX) - 1.0,
                    2.0 * ((float)rand() / (float)RAND_MAX) - 1.0));

                // handle reflection
                const glm::vec3 reflectRay = glm::normalize(glm::reflect(ray.direction, payload.worldNormal));

                // noise around reflection or refraction
                const glm::vec3 noiseR = glm::normalize(glm::vec3(
                    2.0 * ((float) rand() / (float) RAND_MAX) - 1.0,
                    2.0 * ((float) rand() / (float) RAND_MAX) - 1.0,
                    2.0 * ((float) rand() / (float) RAND_MAX) - 1.0));

                // allow refraction
                if (mat.refractionIndex < EPSILON) // opaque material
                {
                    ray.direction = mat.roughness * glm::normalize(payload.worldNormal + noiseN) + (1 - mat.roughness) * glm::normalize(reflectRay + mat.roughness * noiseR);
                    ray.origin = payload.worldPosition + EPSILON * payload.worldNormal;
                }
                else
                {
                    // don't count a  bounce on translucid
                    ray.bounce--;
                    const float cosi1 = glm::dot(ray.direction, -payload.worldNormal);
                    const float i1 = glm::acos(cosi1);
                    float n1;
                    float n2;
                    if (payload.inside)
                    {
                        // the ray is in the sphere
                        n1 = mat.refractionIndex;
                        n2 = Material::AIR_REFRACTION_INDEX;
                    }
                    else
                    {
                        n1 = Material::AIR_REFRACTION_INDEX;
                        n2 = mat.refractionIndex;
                    }

                    const float indexRatio = n2 / n1;

                    // random number
                    const double randomf = (double)std::rand() / RAND_MAX;

                    // schlick approximation (https://en.wikipedia.org/wiki/Schlick%27s_approximation)
                    double r0 = (n1 - n2) / (n1 + n2);
                    r0 *= r0;

                    const double rTheta = r0 + (1 - r0) * glm::pow((1 - cosi1), 5);

                    if (glm::sin(i1) > indexRatio || randomf < rTheta)
                    {
                        // there is total reflexion or the ray is just reflect
                        ray.direction = mat.roughness * glm::normalize(payload.worldNormal + noiseN) + (1 - mat.roughness) * glm::normalize(reflectRay + mat.roughness * noiseR);
                        ray.origin = payload.worldPosition + EPSILON * payload.worldNormal;
                    }
                    else
                    {
                        // the ray is refract
                        ray.direction = glm::refract(ray.direction, payload.worldNormal, n1 / n2);
                        ray.origin = payload.worldPosition - EPSILON * payload.worldNormal;
                    }
                }
            }
            // the color of the current ray
            const glm::vec3 frameColor = glm::vec3(
                light.r * colorContribution.r * 255,
                light.g * colorContribution.g * 255,
                light.b * colorContribution.b * 255);

            // draw the pixel
            accumulatedData[pixelIndex] += frameColor;

            const glm::vec3 outColorVect = glm::clamp(accumulatedData[pixelIndex] / (float)frameId, 0.f, 255.f);

            const ImColor outColor = IM_COL32(
                outColorVect.r,
                outColorVect.g,
                outColorVect.b,
                255);
            imageData[pixelIndex] = outColor;
            //>!!
            //++ imageData[pixelIndex] = IM_COL32((int) ((double) x / (double) getWidth() * 255), (int) ((1. - (double) y / (double) getHeight()) * 255), 0, 255);
#if RESON4
            imageData[(x + 1) + y * getWidth()] = outColor;
            imageData[x + (y + 1) * getWidth()] = outColor;
            imageData[x + 1 + (y + 1) * getWidth()] = outColor;
#endif
        }
    }

    image->setData(imageData);
}

void Raytracing::Renderer::resetAcc()
{
    frameId = 0;
}

void Raytracing::Renderer::setAttenuationFormula(const uint32_t newFormula)
{
    attenuationFormula = newFormula;
}

char *Raytracing::Renderer::getFormulatoString(const uint32_t i) const
{
    switch (i)
    {
    case 1:
        return (char *)"max(0, 1 - d / r)";
    case 2:
        return (char *)"max(0, 1 - (d / r)²)";
    case 3:
        return (char *)"max(0, exp(- (d / r)²))";

    default:
        return (char *)"bad index";
    }
}

Raytracing::HitPayload Raytracing::Renderer::traceRay(Ray *ray) const
{
    //++ // TODO : check intersection with spheres  

    //<!!
    // index
    uint32_t index = 0;
    bool found = false;
    const std::vector<Sphere> list = scene.getListSphere();
    double hitDistance = camera.getFar();

    for (uint32_t i = 0; i < list.size(); i++)
    {
        const Sphere &sphere = list[i];
        const double sphereHitDistance = sphere.intersect(&camera, ray);

        if (sphereHitDistance > 0 && sphereHitDistance < hitDistance)
        {
            hitDistance = sphereHitDistance;
            index = i;
            found = true;
        }
    }
    // handle if any object has been hit
    if (!found)
        return miss();
    // handle if a object has been hit
    // return closestHit(ray, (float) hitDistance, index);
    //>!!
    //++ return HitPayload();
}

Raytracing::HitPayload Raytracing::Renderer::closestHit(Ray *ray, float hitDistance, uint32_t objectIndex) const
{
    //++ // TODO : return payload with data
    // returned struct
    HitPayload payload;
    
    // set the hitDistance
    payload.hitDistance = hitDistance; //!!

    // set the hit sphere
    //<!!
    const std::vector<Sphere> spheres = scene.getListSphere();
    payload.objectIndex = objectIndex;
    const Sphere sphere = spheres[objectIndex];
    //>!!

    // compute the hit position
    payload.worldPosition = ray->origin + hitDistance * ray->direction; //!!
    
    // compute the hit normal (/!\ correct only for sphere).
    //<!!
    payload.worldNormal = glm::normalize(payload.worldPosition - sphere.center);
    const glm::vec3 oc(ray->origin - sphere.center);
    payload.inside = glm::sqrt(glm::dot(oc, oc)) < sphere.radius;
    //>!!
    
    // revert the normal if the bounce is inside the sphere
    //<!!
    if (payload.inside)
        payload.worldNormal *= -1;
    //>!!

    return payload;
}

Raytracing::HitPayload Raytracing::Renderer::miss() const
{
    //++ // TODO : return a empty (or with negative distance) payload
    // returned the payload
    Raytracing::HitPayload payload;
    payload.hitDistance = -1; //!!
    return payload;
}

float Raytracing::Renderer::getAttenuation(const HitPayload payload, const Material mat) const
{
    // light attenuation
    //++ // TODO : fit the getFormulatoString to add a light attenuation phenomen
    //<!!
    const double dOnR = payload.hitDistance / mat.attenuationRadius;
    const double a = (1. - dOnR * dOnR);
    //>!!
    switch (attenuationFormula)
    {
    //<!!
    case 1:
        return glm::max(0.f, (float) (1. - dOnR));
    case 2:
        return glm::max(0.f, (float) a);
    case 4:
        return glm::exp((float) (-dOnR * dOnR));
    //>!!
    default:
        return 1.;
    }
}