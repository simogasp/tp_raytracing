
# include "imgui.h"
# include "Renderer.hpp"
# include <stdio.h>

Raytracing::Renderer::~Renderer()
{
    free(image);
}

// render every pixel of the screen.
void Raytracing::Renderer::Render()
{
    for (size_t y = 0; y < getHeight(); y++)
    {
        for (size_t x = 0; x < getWidth(); x++)
        {
            image->setPixel(x + y * getWidth(), perPixel(x, y));
        }        
    }
    
    {
    }

    image->update();
}


void Raytracing::Renderer::OnResize(const uint32_t newWidth, const uint32_t newHeight)
{
    if (image && image->getWidth() == newWidth && image->getHeight() == newHeight)
        return;
    image->resize(newWidth, newHeight);
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

uint32_t Raytracing::Renderer::perPixel(const uint32_t x, const uint32_t y)
{
    return IM_COL32((float) x / getWidth() * 255, (1 - (float) y / getHeight()) * 255, 0, 255);
}
