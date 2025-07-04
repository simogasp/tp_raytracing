#pragma once

#include "ImageWrapper.hpp"
#include "raytracing/core/Scene.hpp"
#include "raytracing/core/Camera.hpp"

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
    private:
        ImageWrapper *image;
        Scene scene;
        Camera camera;
        float screenDistance;
        Ray *rays;

    public:
        Renderer();
        ~Renderer();

        uint32_t getWidth();
        uint32_t getHeight();
        GLuint getTextureId();

        void OnResize(const uint32_t newWidth, const uint32_t newHeight);
        void Render();

    private:
        void setRayDirection();
        uint32_t perPixel(const uint32_t x, const uint32_t y);
    };

}
