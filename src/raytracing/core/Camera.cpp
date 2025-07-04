#include "Camera.hpp"
#include "Ray.hpp"

Raytracing::Camera::Camera(const Vector3 &positionCamera, const Vector3 &lookAtCamera,
        const Vector3 &upCamera, const float focalCamera, const float fieldOfViewCamera,
        const float nearCamera, const float farCamera)
{
    position = positionCamera;
    lookAt = lookAtCamera;
    up = upCamera;
    focal = focalCamera;
    fieldOfView = clampi(fieldOfViewCamera, 1, 359);
    near = nearCamera;
    far = farCamera;
}

Raytracing::Camera::~Camera()
{
}

Vector3 Raytracing::Camera::getPosition()
{
    return position;
}

Vector3 Raytracing::Camera::getLookAt()
{
    return lookAt;
}

float Raytracing::Camera::getFocal()
{
    return focal;
}

float Raytracing::Camera::getFieldOfView()
{
    return fieldOfView;
}

float Raytracing::Camera::getNear()
{
    return near;
}

float Raytracing::Camera::getFar()
{
    return far;
}
