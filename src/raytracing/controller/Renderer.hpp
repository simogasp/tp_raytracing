# pragma once

# include "ImageWrapper.hpp"
# include "raytracing/core/Scene.hpp"

# include <stdint.h>

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
        Raytracing::ImageWrapper* image;
        Raytracing::Scene scene;
    public:
        Renderer();
        ~Renderer();

        void OnResize(const uint32_t newWidth, const uint32_t newHeight);
        void Render();

        uint32_t getWidth();
        uint32_t getHeight();
        GLuint getTextureId();

    private:
        uint32_t perPixel(const uint32_t x, const uint32_t y);
    };

}
