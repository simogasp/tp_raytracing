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
        int pixel;

    public:
        Renderer();
        ~Renderer();

        uint32_t getWidth();
        uint32_t getHeight();
        GLuint getTextureId();

        void onResize(const uint32_t newWidth, const uint32_t newHeight);
        void Render(const Scene& renderedScene, const Camera& renderingCamera);

        private:
        uint32_t traceRay(Raytracing::Ray * ray);
    };

}
