#include "ImageWrapper.hpp"
#include <cstdio>
#include <iostream>

unsigned int Raytracing::ImageWrapper::getWidth() const
{
    return width;
}

unsigned int Raytracing::ImageWrapper::getHeight() const
{
    return height;
}

GLuint Raytracing::ImageWrapper::getTextureId() const
{
    return textureId;
}

Raytracing::ImageWrapper::ImageWrapper()
{
    width = 0;
    height = 0; 
    textureId = 0;
    imageData = nullptr;
}

void Raytracing::ImageWrapper::setData(uint32_t *newData)
{
    imageData = newData;

    if (textureId == 0 || width == 0 || height == 0)
        return;

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (int) width, (int) height, GL_RGBA, GL_UNSIGNED_BYTE, newData);
}

void Raytracing::ImageWrapper::resize(uint32_t newWidth, uint32_t newHeight)
{
    if (width == newWidth && newHeight == height)
    {
        return;
    }

    width = newWidth;
    height = newHeight;


    if (textureId == 0)
    {
        glGenTextures(1, &textureId);
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) width, (int) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
