# include "ImageWrapper.hpp"
# include <stdio.h>

uint Raytracing::ImageWrapper::getWidth()
{
    return width;
}

uint Raytracing::ImageWrapper::getHeight()
{
    return height;
}

GLuint Raytracing::ImageWrapper::getTextureId()
{
    return textureId;
}

void Raytracing::ImageWrapper::setPixel(const int i, const uint32_t color)
{
    imageData[i] = color;
    // printf("set imageData[%d, %d] = %0x\n", i / width, i % height, color);
}

void Raytracing::ImageWrapper::update() 
{
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}



void Raytracing::ImageWrapper::resize(const uint newWidth, const uint newHeight)
{
    // resize GPU image
    delete[] imageData;

    // update dims
    width = newWidth;
    height = newHeight;
    imageData = new uint32_t[width * height];

    if (textureId == 0) {
        glGenTextures(1, &textureId);
    }

    update();
}

