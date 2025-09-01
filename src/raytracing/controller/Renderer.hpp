#pragma once

#include "ImageWrapper.hpp"
#include "raytracing/core/Scene.hpp"
#include "raytracing/core/Camera.hpp"
#include "raytracing/core/HitPayload.hpp"
#include <cstdint>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

namespace Raytracing
{

    class Renderer
    {
    public:
        const float EPSILON = 1e-2f;

    private:
        /**
         * The GPU image wrapper.
         */
        ImageWrapper *image;
        /**
         * The image data.
         */
        uint32_t *imageData;

        /**
         * The accumulated data of pixel.
         */
        glm::vec3 *accumulatedData;

        /**
         * The id of the rendered Framed.
         */
        unsigned int frameId;
        /**
         * The last scene given to render.
         */
        Scene scene;
        /**
         * The last camera given to render.
         */
        Camera camera;
        /**
         * The actual light attenuation Formula selected.
         */
        uint32_t attenuationFormula;

    public:
        /**
         * Creates a Renderer.
         */
        Renderer();
        
        /**
         * Destroys a Renderer.
         */
        ~Renderer() = default;
        

        /**
         * Gets the width of the image.
         * @return the width
         */
        [[nodiscard]] uint32_t getWidth() const;
        
        /**
         * Gets the height of the image.
         * @return the height
         */
        [[nodiscard]] uint32_t getHeight() const;

        /**
         * Gets the FrameId of the image.
         * @return the FrameId
         */
        uint32_t getFrameId() const;
        
        /**
         * Gets the GPU texture ID to load the image.
         * @return the texture id
         */
        [[nodiscard]] GLuint getTextureId() const;


        /**
         * Handles a resive of the screen.
         */
        void onResize(const uint32_t newWidth, const uint32_t newHeight);

        /**
         * Create a Render of the screen.
         */
        void Render(const Scene &renderedScene, const Camera &renderingCamera);

        /**
         * Reset the accumulator.
         */
        void resetAcc();
        
        /**
         * Change the light attenuation Formula.
         */
        void setAttenuationFormula(const uint32_t newFormula);

        /**
         * Gets the i-th formula as string (hardcoded).
         * @return the formula as a string
         */
        char *getFormulatoString(const uint32_t i) const;

    private:
        /**
         * Traces the given ray.
         * @return a hit payload with usefull information.
         */
        HitPayload traceRay(Ray *ray) const;

        /**
         * Creates a payload according to the closest hit of the ray.
         * @param ray the ray to handle
         * @param hitDistance the distance of the hit
         * @param ObjectIndex the object index hit
         * @return the payload with the hit information
         */
        HitPayload closestHit(Ray *ray, float hitDistance, uint32_t ObjectIndex) const;

        /**
         * Creates a payload for ray with any hit.
         * @return the payload.
         */
        [[nodiscard]] HitPayload miss() const;
        
        /**
         * Gets the attenuation factor for a object light.
         * @return the attenuation
         */
        [[nodiscard]] float getAttenuation(const HitPayload payload, const Material mat) const;
    };

}
