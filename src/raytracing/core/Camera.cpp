#include "Camera.hpp"
#include "Ray.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace
{
    glm::vec3 rotateAroundAxis(const glm::vec3& vector, const glm::vec3& axis, float angle)
    {
        const glm::vec3 normalizedAxis = glm::normalize(axis);
        const float cosAngle = std::cos(angle);
        const float sinAngle = std::sin(angle);
        return vector * cosAngle
            + glm::cross(normalizedAxis, vector) * sinAngle
            + normalizedAxis * glm::dot(normalizedAxis, vector) * (1.f - cosAngle);
    }
}

Raytracing::Camera::Camera(const glm::vec3& positionCamera,
                           const glm::vec3& lookAtCamera,
                           const glm::vec3& upCamera,
                           float focalCamera,
                           float nearCamera,
                           float farCamera)
  : position(positionCamera)
  , lookAt(lookAtCamera)
  , upVector(upCamera)
  , near(nearCamera)
  , far(farCamera)
  , focal(focalCamera)
{
    computeBase();
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

const std::vector<glm::vec3>& Raytracing::Camera::getRayDirections() const
{
    return rayDirections;
}

void Raytracing::Camera::setFocal(float newValue)
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

void Raytracing::Camera::setNear(double newNear)
{
    near = static_cast<float>(newNear);
}

void Raytracing::Camera::setFar(double newFar)
{
    far = static_cast<float>(newFar);
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
    z = glm::normalize(rotateAroundAxis(z, x, -rotationSpeed));
    y = glm::normalize(glm::cross(z, x));
    upVector = y;
    lookAt = position + z;
    updateRay();
}

void Raytracing::Camera::lookDown()
{
    z = glm::normalize(rotateAroundAxis(z, x, rotationSpeed));
    y = glm::normalize(glm::cross(z, x));
    upVector = y;
    lookAt = position + z;
    updateRay();
}

void Raytracing::Camera::lookLeft()
{
    x = glm::normalize(rotateAroundAxis(x, y, rotationSpeed));
    z = glm::normalize(rotateAroundAxis(z, y, rotationSpeed));
    lookAt = position + z;
    updateRay();
}

void Raytracing::Camera::lookRight()
{
    x = glm::normalize(rotateAroundAxis(x, y, -rotationSpeed));
    z = glm::normalize(rotateAroundAxis(z, y, -rotationSpeed));
    lookAt = position + z;
    updateRay();
}

void Raytracing::Camera::rotateClockWise()
{
    x = glm::normalize(rotateAroundAxis(x, z, rotationSpeed));
    y = glm::normalize(glm::cross(z, x));
    upVector = y;
    updateRay();
}

void Raytracing::Camera::rotateAntiClockWise()
{
    x = glm::normalize(rotateAroundAxis(x, z, -rotationSpeed));
    y = glm::normalize(glm::cross(z, x));
    upVector = y;
    updateRay();
}

void Raytracing::Camera::onResize(uint32_t newWidth, uint32_t newHeight)
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
    const float invScreenRatio =  static_cast<float>(height) / static_cast<float>(width);

    const glm::vec3 screenPos = focal * glm::normalize(lookAt - position);

    #pragma omp parallel for collapse(2)
    for (uint32_t py = 0; py < height; py++)
    {
        for (uint32_t px = 0; px < width; px++)
        {
            // relative placement on the screen of the pixel
            const float relativeX = - 2 * static_cast<float>(px) / static_cast<float>(width - 1) + 1;
            const float relativeY = - 2 * static_cast<float>(py) / static_cast<float>(height - 1) + 1;

            rayDirections[px + width * py] = glm::normalize(screenPos + relativeX * x + relativeY * y * invScreenRatio);
        }

    }
}

glm::vec3 Raytracing::Camera::baseChangment(glm::vec3 vect) const
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
