# pragma once

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
        uint32_t width;
        uint32_t height;
        GLuint textureId;
        uint32_t * imageData = nullptr;

    public:
        Renderer() = default;
        ~Renderer() = default;

        void OnResize(const uint32_t newWidth, const uint32_t newHeight);
        void Render();

        uint32_t getWidth();
        uint32_t getHeight();
        GLuint getTextureId();

    private:
    
    };

}
