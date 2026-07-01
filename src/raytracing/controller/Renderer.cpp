
#include "imgui.h"
#include "Renderer.hpp"
#include "raytracing/core/Ray.hpp"
#include "raytracing/core/HitPayload.hpp"
#include <glm/gtc/constants.hpp>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <random>
#include <cmath>

// number of bounce to made
#define BOUNCES 5

// devide the resolution by 4 to accelerate the computation
#define RESON4 1

// define a minimum value for refractive materials
#define REFRACTION_MIN 1e-4f

// define a minimum value under which we consider that a ray hits an object
#define HIT_EPSILON 1e-5f

// define a wider marching threshold before refining the hit position.
// Grazing rays need too many iterations to reach HIT_EPSILON directly.
#define RAY_MARCH_HIT_EPSILON 1e-6f

// make the marching hit threshold slightly grow with distance to avoid
// horizon-like banding on very shallow floor rays.
#define RAY_MARCH_RELATIVE_HIT_EPSILON 2e-4f

// define a small bias to avoid re-hitting the same surface after a bounce
#define RAY_BIAS 1e-4f

// define a maximum amount of steps that can be taken along a ray
#define MAX_STEPS 512

// define a maximum amount of material interactions for a ray.
// Translucid materials do not consume a bounce, so this prevents very long
// reflection/refraction chains inside glass objects.
#define MAX_RAY_EVENTS 32

// define how many times we refine a raymarched hit position.
// This reduces visible quantization artifacts on refractive surfaces.
#define HIT_REFINE_ITERATIONS 4

namespace
{
    struct SceneSdfSample
    {
        float signedDistance;
        float surfaceDistance;
        uint32_t objectIndex;
    };

    float boundingBoxDistanceSquared(const Raytracing::AxisAlignedBoundingBox& boundingBox,
                                     const glm::vec3& position)
    {
        const glm::vec3 outside = glm::max(
            glm::max(boundingBox.min - position, position - boundingBox.max),
            glm::vec3(0.f));
        return glm::dot(outside, outside);
    }

    Raytracing::SdfObjectList buildSdfObjects(const Raytracing::Scene::ObjectList& objects)
    {
        Raytracing::SdfObjectList sdfObjects;
        sdfObjects.reserve(objects.size());

        for (uint32_t i = 0; i < objects.size(); i++)
        {
            const Raytracing::HittableObject* object = objects[i].get();
            const bool hasBoundingBox = object->hasBoundingBox();
            sdfObjects.push_back({
                object,
                hasBoundingBox ? object->getBoundingBox() : Raytracing::AxisAlignedBoundingBox{},
                i,
                hasBoundingBox
            });
        }

        return sdfObjects;
    }

    SceneSdfSample sampleSceneSdf(const Raytracing::SdfObjectList& objects,
                                  const glm::vec3& position,
                                  float maxDistance)
    {
        SceneSdfSample closest {
            maxDistance,
            maxDistance,
            0
        };

        for (const Raytracing::SdfObject& object : objects)
        {
            if (object.hasBoundingBox)
            {
                const float bestDistanceSquared = closest.surfaceDistance * closest.surfaceDistance;
                if (boundingBoxDistanceSquared(object.boundingBox, position) >= bestDistanceSquared)
                    continue;
            }

            const float signedDistance = static_cast<float>(object.object->sdf(position));
            const float surfaceDistance = std::abs(signedDistance);

            if (surfaceDistance < closest.surfaceDistance)
            {
                closest.signedDistance = signedDistance;
                closest.surfaceDistance = surfaceDistance;
                closest.objectIndex = object.objectIndex;
            }
        }

        return closest;
    }

    float refineHitDistance(const Raytracing::Ray& ray, const Raytracing::HittableObject& object, float hitDistance)
    {
        float refinedHitDistance = hitDistance;

        for (int i = 0; i < HIT_REFINE_ITERATIONS; i++)
        {
            const glm::vec3 position = ray.origin + refinedHitDistance * ray.direction;
            const float signedDistance = static_cast<float>(object.sdf(position));

            if (std::abs(signedDistance) < HIT_EPSILON * 0.25f)
                break;

            const glm::vec3 normal = object.getNormal(position);
            const float rayNormalProjection = glm::dot(normal, ray.direction);

            if (std::abs(rayNormalProjection) < 1e-4f)
                break;

            refinedHitDistance -= signedDistance / rayNormalProjection;
            refinedHitDistance = glm::max(refinedHitDistance, 0.f);
        }

        return refinedHitDistance;
    }

    glm::vec3 getTranslucentTransmission(const Raytracing::Material& material, float travelDistance)
    {
        const glm::vec3 tint = glm::clamp(material.reflection, glm::vec3(0.f), glm::vec3(1.f));
        const float absorption = 1.f - std::exp(-travelDistance * material.translucentAbsorption);
        return glm::mix(glm::vec3(1.f), tint, absorption);
    }
}

Raytracing::Renderer::Renderer()
{
    image = new ImageWrapper();
    attenuationFormula = 1;

    omp_set_num_threads(omp_get_num_procs());
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

    // get the computed ray direction from camera
    const std::vector<glm::vec3>& dirs = renderingCamera.getRayDirections();
    const SdfObjectList sdfObjects = buildSdfObjects(renderedScene.getListObjects());

    // random generator for noise
    const size_t numThreads = static_cast<size_t>(omp_get_max_threads());
    std::vector<std::mt19937> rngs(numThreads);

    std::random_device rd;
    for (auto &rng : rngs) {
        rng.seed(rd());
    }

#pragma omp parallel
{
    //#pragma omp single
    //{
    //    std::cout << "Threads OpenMP: " << omp_get_num_threads() << std::endl;
    //}
#if RESON4
    #pragma omp for schedule(dynamic, 1)
    for (size_t y = 0; y < getHeight(); y += 2)
#else
    #pragma omp for schedule(dynamic, 1)
    for (size_t y = 0; y < getHeight(); y++)
#endif
    {
        const size_t tid = static_cast<size_t>(omp_get_thread_num());
        auto &rng = rngs[tid];
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
            ray.origin = renderingCamera.getPosition();
            ray.direction = dirs[pixelIndex];
            ray.bounce = 0;
            ray.mediumMaterialIndex = -1;

            // final color by substraction method
            glm::vec3 light(0.f);
            glm::vec3 colorContribution(1.f);
            float shiny = 1.f;

            // iteration on bounce
            for (int rayEvents = 0; ray.bounce <= BOUNCES && rayEvents < MAX_RAY_EVENTS; ray.bounce++, rayEvents++)
            {
                HitPayload payload = rayMarch(&ray, renderedScene, sdfObjects, renderingCamera.getFar());

                if (payload.hitDistance < 0)
                {
                    // we missed all spheres
                    const double rgColor = (1. - fabs(ray.direction.y)) / 2 + 0.3;
                    const glm::vec3 skyColor(rgColor, rgColor, 1);
                    light += skyColor;
                    break;
                }

                // update the color
                const HittableObject& object = *renderedScene.getListObjects()[payload.objectIndex];
                const std::vector<Material>& materials = renderedScene.getListMaterial();
                if (ray.mediumMaterialIndex >= 0)
                    colorContribution *= getTranslucentTransmission(materials[static_cast<size_t>(ray.mediumMaterialIndex)], payload.hitDistance);

                const Material& mat = materials[object.getMaterialIndex()];
                colorContribution = (1.f - shiny) * colorContribution + shiny * mat.reflection;
                shiny *= mat.shinyness;

                if (ray.bounce > 0)
                    light += getAttenuation(payload, mat) * mat.getEmission();
                else
                    light += mat.getEmission();

                const glm::vec3 surfaceNormal = payload.inside ? -payload.worldNormal : payload.worldNormal;

                // noise around normal
                std::uniform_real_distribution<float> dist(-1.f, 1.f);
                
                const glm::vec3 noiseN = glm::normalize(glm::vec3(
                    dist(rng),
                    dist(rng),
                    dist(rng)
                ));

                // handle reflection
                const glm::vec3 reflectRay = glm::normalize(glm::reflect(ray.direction, surfaceNormal));

                // noise around reflection or refraction
                const glm::vec3 noiseR = glm::normalize(glm::vec3(
                    dist(rng),
                    dist(rng),
                    dist(rng)
                ));

                // allow refraction
                if (mat.refractionIndex < REFRACTION_MIN) // opaque material
                {
                    ray.direction = glm::normalize(mat.roughness * glm::normalize(surfaceNormal + noiseN) + (1 - mat.roughness) * glm::normalize(reflectRay + mat.roughness * noiseR));
                    ray.origin = payload.worldPosition + RAY_BIAS * surfaceNormal;
                }
                else
                {
                    // don't count a  bounce on translucid
                    ray.bounce--;
                    const float cosi = glm::clamp(glm::dot(-ray.direction, surfaceNormal), 0.0f, 1.0f);

                    float n1, n2;
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

                    const float eta = n1 / n2;

                    // --- TIR test (without acos/sin) ---
                    float sin2_t = eta * eta * (1.0f - cosi * cosi);

                    bool tir = sin2_t > 1.0f;

                    // schlick approximation (https://en.wikipedia.org/wiki/Schlick%27s_approximation)
                    // --- Schlick reflection probability ---
                    float R0 = (n1 - n2) / (n1 + n2);
                    R0 = R0 * R0;

                    float R = R0 + (1.0f - R0) * powf(1.0f - cosi, 5.0f);

                    // random number
                    std::uniform_real_distribution<float> dist01(0.f, 1.f);
                    float randomf = dist01(rng);
                    
                    if (tir || randomf < R)
                    {
                        // REFLECTION
                        ray.direction = glm::normalize(glm::reflect(ray.direction, surfaceNormal));
                        ray.origin = payload.worldPosition + RAY_BIAS * surfaceNormal;
                    }
                    else
                    {
                        // REFRACTION
                        ray.direction = glm::normalize(glm::refract(ray.direction, surfaceNormal, n1 / n2));
                        ray.origin = payload.worldPosition - RAY_BIAS * surfaceNormal;
                        ray.mediumMaterialIndex = payload.inside ? -1 : static_cast<int>(object.getMaterialIndex());
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

Raytracing::HitPayload Raytracing::Renderer::rayMarch(Ray *ray, const Scene& renderedScene, const SdfObjectList& sdfObjects, float maxDistance) const
{
    //++ // TODO : check intersection with spheres  

    //<!!
    // index
    uint32_t index = 0;
    bool found = false;
    const float maxDst = maxDistance;
    float hitDistance = 0.f;
    glm::vec3 position_on_ray = ray->origin;

    int step = 0;

    while(step <= MAX_STEPS && hitDistance < maxDst) 
    {
        step++;

        const SceneSdfSample closest = sampleSceneSdf(sdfObjects, position_on_ray, maxDst);
        const float hitThreshold = glm::max(
            RAY_MARCH_HIT_EPSILON,
            hitDistance * RAY_MARCH_RELATIVE_HIT_EPSILON);

        if(closest.surfaceDistance < hitThreshold && hitDistance > RAY_BIAS)
        {
            const HittableObject& closestObject = *sdfObjects[closest.objectIndex].object;
            const glm::vec3 normal = closestObject.getNormal(position_on_ray);
            const float rayNormalProjection = glm::dot(normal, ray->direction);

            if (closest.signedDistance * rayNormalProjection < 0.f)
            {
                found = true;
                index = closest.objectIndex;
                break;
            }
        }
    
        const float stepDistance = glm::max(closest.surfaceDistance, HIT_EPSILON);

        hitDistance += stepDistance;
        position_on_ray += stepDistance * ray->direction;

    }

    if(hitDistance < maxDst && hitDistance < HIT_EPSILON) {
        found = true;
    }

    // handle if any object has been hit
    if (!found)
        return miss();
    // handle if an object has been hit
    return closestHit(ray, renderedScene, hitDistance, index);
    //>!!
    //++ return HitPayload();
}

Raytracing::HitPayload Raytracing::Renderer::closestHit(Ray *ray, const Scene& renderedScene, float hitDistance, uint32_t objectIndex) const
{
    //++ // TODO : return payload with data
    // returned struct
    HitPayload payload;
    
    // set the hit object
    //<!!
    const Scene::ObjectList& objects = renderedScene.getListObjects();
    payload.objectIndex = objectIndex;
    const HittableObject& object = *objects[objectIndex];
    //>!!

    // set the hitDistance
    payload.hitDistance = refineHitDistance(*ray, object, hitDistance); //!!

    // compute the hit position
    payload.worldPosition = ray->origin + payload.hitDistance * ray->direction; //!!
    
    // compute the hit normal (/!\ correct only for sphere).
    //<!!
    payload.worldNormal = object.getNormal(payload.worldPosition);
    payload.inside = glm::dot(ray->direction, payload.worldNormal) > 0;
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

float Raytracing::Renderer::getAttenuation(const HitPayload& payload, const Material& mat) const
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
