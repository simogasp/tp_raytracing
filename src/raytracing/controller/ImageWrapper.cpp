#include "ImageWrapper.hpp"
#include <stdio.h>
#include <iostream>

unsigned int Raytracing::ImageWrapper::getWidth() const
{
    return width;
}

unsigned int Raytracing::ImageWrapper::getHeight() const
{
    return height;
}

GLuint Raytracing::ImageWrapper::getTextureId()
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
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) width, (int) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Raytracing::ImageWrapper::resize(const uint32_t newWidth, const uint32_t newHeight)
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
}
