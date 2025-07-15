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

    class Renderer
    {
    public:
        const float EPSILON = 1e-3;

    private:
        ImageWrapper *image;
        uint32_t *imageData;
        glm::vec4 *accumulatedData;
        uint FrameId;
        Scene scene;
        Camera camera;

    public:
        Renderer();
        ~Renderer() = default;
        uint32_t getWidth();
        uint32_t getHeight();
        GLuint getTextureId();

        void onResize(const uint32_t newWidth, const uint32_t newHeight);
        void Render(const Scene &renderedScene, const Camera &renderingCamera);
        void resetAcc();

    private:
        Raytracing::HitPayload traceRay(Ray *ray);
        Raytracing::HitPayload closestHit(Ray *ray, float hitDistance, int ObjectIndex);
        Raytracing::HitPayload miss(Ray *ray);
    };

}
