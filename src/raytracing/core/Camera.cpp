#include "Camera.hpp"
#include "Ray.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Raytracing::Camera::Camera(const glm::vec3 &positionCamera, const glm::vec3 &lookAtCamera,
                           const glm::vec3 &upCamera, const float focalCamera,
                           const float nearCamera, const float farCamera)
{
    position = positionCamera;
    lookAt = lookAtCamera;
    upVector = upCamera;
    focal = focalCamera;
    near = nearCamera;
    far = farCamera;
    computeBase();
}

Raytracing::Camera::~Camera()
{
}

glm::vec3 Raytracing::Camera::getPosition() const
{
    return position;
}

glm::vec3 Raytracing::Camera::getLookAt() const
{
    return lookAt;
}

float Raytracing::Camera::getFocal() const
{
    return focal;
}

float Raytracing::Camera::getNear() const
{
    return near;
}

float Raytracing::Camera::getFar() const
{
    return far;
}

std::vector<glm::vec3> Raytracing::Camera::getRayDirections() const
{
    return rayDirections;
}

void Raytracing::Camera::setFocal(const float newValue)
{
    focal = newValue;
    updateRay();
}

void Raytracing::Camera::setUpVector(const glm::vec3 &newUp)
{
    upVector = newUp;
    computeBase();
}

void Raytracing::Camera::setPosition(const glm::vec3 &newPosition)
{
    position = newPosition;
    computeBase();
}

void Raytracing::Camera::setLookAt(const glm::vec3 &newLookAt)
{
    lookAt = newLookAt;
    computeBase();
}

void Raytracing::Camera::setNear(const double newNear)
{
    near = (float) newNear;
}

void Raytracing::Camera::setFar(const double newFar)
{
    far = (float) newFar;
}

void Raytracing::Camera::forward()
{
    position += speed * z;
    lookAt += speed * z;
}

void Raytracing::Camera::backward()
{
    position -= speed * z;
    lookAt -= speed * z;
}

void Raytracing::Camera::left()
{
    position += speed * x;
    lookAt += speed * x;
}

void Raytracing::Camera::right()
{
    position -= speed * x;
    lookAt -= speed * x;
}

void Raytracing::Camera::up()
{
    position += speed * y;
    lookAt += speed * y;
}

void Raytracing::Camera::down()
{
    position -= speed * y;
    lookAt -= speed * y;
}

void Raytracing::Camera::lookUp()
{
    glm::mat3 rot(
        1, 0, 0,
        0, cosRotationSpeed, sinRotationSpeed,
        0, - sinRotationSpeed, cosRotationSpeed);
    upVector = rot * upVector;
    z = rot * z;
    y = rot * y;
    lookAt = position + z;
}

void Raytracing::Camera::lookDown()
{
    glm::mat3 rot(
        1, 0, 0,
        0, cosRotationSpeed, - sinRotationSpeed,
        0, sinRotationSpeed, cosRotationSpeed);
    upVector = rot * upVector;
    z = rot * z;
    y = rot * y;
    lookAt = position + z;
}

void Raytracing::Camera::lookLeft()
{
    glm::mat3 rot(
        cosRotationSpeed, 0, - sinRotationSpeed,
        0, 1, 0,
        sinRotationSpeed, 0, cosRotationSpeed);
    x = rot * x;
    z = rot * z;
    lookAt = position + z;
}

void Raytracing::Camera::lookRight()
{
    glm::mat3 rot(
        cosRotationSpeed, 0, sinRotationSpeed,
        0, 1, 0,
        - sinRotationSpeed, 0, cosRotationSpeed);
    x = rot * x;
    z = rot * z;
    lookAt = position + z;
}

void Raytracing::Camera::rotateClockWise()
{
    glm::mat3 rot(
        cosRotationSpeed, sinRotationSpeed, 0,
        -sinRotationSpeed, cosRotationSpeed, 0,
        0, 0, 1);
    upVector = rot * upVector;
    computeBase();
}

void Raytracing::Camera::rotateAntiClockWise()
{
    glm::mat3 rot(
        cosRotationSpeed, -sinRotationSpeed, 0,
        sinRotationSpeed, cosRotationSpeed, 0,
        0, 0, 1);
    upVector = rot * upVector;
    computeBase();
}

void Raytracing::Camera::onResize(const uint32_t newWidth, const uint32_t newHeight)
{

    if (width == newWidth && height == newHeight)
        return;
    width = newWidth;
    height = newHeight;
    rayDirections.resize(width * height);
    updateRay();
}

void Raytracing::Camera::updateRay()
{
    //++ // TODO : orient the ray depending on the focal
    //<!!
    const float invScreenRatio = (float)height / (float)width;

    const glm::vec3 screenPos = focal * glm::normalize(lookAt - position);

    #pragma omp parallel for collapse(2)
    for (uint32_t py = 0; py < height; py++)
    {
        for (uint32_t px = 0; px < width; px++)
        {
            // relative placement on the screen of the pixel
            const float relativeX = - 2 * (float)px / (float)(width - 1) + 1;
            const float relativeY = - 2 * (float)py / (float)(height - 1) + 1;

            rayDirections[px + width * py] = glm::normalize(screenPos + relativeX * x + relativeY * y * invScreenRatio);
        }

    }
    //>!!
}

glm::vec3 Raytracing::Camera::baseChangment(glm::vec3 vect)
{
    const glm::mat3 mat(x, y, z);
    const glm::vec3 res(mat * vect);
    return res;
}

// compute base
void Raytracing::Camera::computeBase()
{
    z = glm::normalize(lookAt - position);
    x = glm::normalize(glm::cross(upVector, z));
    y = glm::cross(z, x);
    updateRay();
}
