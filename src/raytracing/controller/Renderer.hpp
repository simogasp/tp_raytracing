#pragma once

#include "ImageWrapper.hpp"
#include "raytracing/core/Scene.hpp"
#include "raytracing/core/Camera.hpp"
#include "raytracing/core/HitPayload.hpp"
#include <stdint.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

namespace Raytracing
{

    /**
     * @class Renderer
     * @brief Handles the rendering process for raytracing, managing image data, scene, camera, and attenuation formulas.
     *
     * The Renderer class is responsible for creating and managing the rendering pipeline, including image buffers,
     * accumulation data, and rendering logic. It provides methods to resize the output, perform rendering, reset accumulation,
     * and configure attenuation formulas. The class also encapsulates raytracing operations such as tracing rays,
     * handling hits and misses, and calculating attenuation based on material properties.
     *
     * @note EPSILON is used for floating-point comparisons in raytracing calculations.
     *
     * @section Public Methods
     * - Renderer(): Constructs a Renderer instance.
     * - ~Renderer(): Destroys the Renderer instance.
     * - getWidth(): Returns the width of the rendered image.
     * - getHeight(): Returns the height of the rendered image.
     * - getTextureId(): Retrieves the OpenGL texture ID for the rendered image.
     * - onResize(newWidth, newHeight): Resizes the rendering buffers to new dimensions.
     * - Render(renderedScene, renderingCamera): Executes the rendering process for the given scene and camera.
     * - resetAcc(): Resets the accumulation buffer.
     * - setAttenuationFormula(newFormula): Sets the attenuation formula used in rendering.
     * - getAttenuationFormula(): Gets the current attenuation formula.
     * - getFormulatoString(i): Returns a string representation of the attenuation formula.
     *
     * @section Private Methods
     * - traceRay(ray): Traces a ray and returns hit payload information.
     * - closestHit(ray, hitDistance, ObjectIndex): Handles the closest hit logic for a ray.
     * - miss(ray): Handles the miss logic for a ray.
     * - getAttenuation(payload, mat): Calculates attenuation based on hit payload and material.
     */
    class Renderer
    {
    public:
        const float EPSILON = 1e-3;

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
        uint frameId;
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
        uint attenuationFormula;

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
        uint32_t getWidth();
        /**
         * Gets the height of the image.
         * @return the height
         */
        uint32_t getHeight();
        /**
         * Gets the GPU texture ID to load the image.
         * @return the texture id
         */
        GLuint getTextureId();

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
        void setAttenuationFormula(const uint newFormula);

        /**
         * Gets the i-th formula as string (hardcoded).
         * @return the formula as a string
         */
        char *getFormulatoString(const int i);

    private:
        /**
         * Traces the given ray.
         * @return a hit payload with usefull information.
         */
        Raytracing::HitPayload traceRay(Ray *ray);
        /**
         * Creates a payload according to the closest hit of the ray.
         */
        Raytracing::HitPayload closestHit(Ray *ray, float hitDistance, int ObjectIndex);
        /**
         * Creates a payload for ray with any hit.
         */
        Raytracing::HitPayload miss(Ray *ray);
        /**
         * Gets the attenuation factor for a object light.
         */
        float getAttenuation(const HitPayload payload, const Material mat);
    };

}
