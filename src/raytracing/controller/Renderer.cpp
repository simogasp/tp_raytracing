
#include "Renderer.hpp"


// render every pixel of the screen.
void Raytracing::Renderer::Render()
{

    for (size_t i = 0; i < width * height; i++)
    {
        imageData[i] = 0xFF000000 | (int)(0x00FFFFFF * ((float)rand() / RAND_MAX));
    }
}

void Raytracing::Renderer::OnResize(const uint32_t newWidth, const uint32_t newHeight)
{
    if (imageData && width == newWidth && height == newHeight)
        return;

    // resize GPU image
    delete[] imageData;
    imageData = new uint32_t[width * height];

    if (textureId == 0) {
        glGenTextures(1, &textureId);
    }
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

uint32_t Raytracing::Renderer::getWidth()
{
    return width;
}

uint32_t Raytracing::Renderer::getHeight()
{
    return height;
}

GLuint Raytracing::Renderer::getTextureId()
{
    return textureId;
}
