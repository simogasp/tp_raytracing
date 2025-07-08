#include "Camera.hpp"
#include "Ray.hpp"

Raytracing::Camera::Camera(const Vector3 &positionCamera, const Vector3 &lookAtCamera,
        const Vector3 &upCamera, const float focalCamera, const float fieldOfViewCamera,
        const float nearCamera, const float farCamera)
{
    position = positionCamera;
    lookAt = lookAtCamera;
    upVector = upCamera;
    focal = focalCamera;
    fieldOfView = std::clamp((double) fieldOfViewCamera, 0.0, 2.0 * g_pi);
    near = nearCamera;
    far = farCamera;

    computeBase();
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

Vector3 Raytracing::Camera::baseChangment(Vector3 vect)
{
    return Vector3(
        x.m_x * vect.m_x + x.m_y * vect.m_y + x.m_z * vect.m_z,
        y.m_x * vect.m_x + y.m_y * vect.m_y + y.m_z * vect.m_z,
        z.m_x * vect.m_x + z.m_y * vect.m_y + z.m_z * vect.m_z
    );
}

void Raytracing::Camera::setFov(const double newValue)
{
    fieldOfView = newValue;
}

void Raytracing::Camera::forward()
{
    position += speed * z;
    lookAt += speed * z;
}

void Raytracing::Camera::backward()
{
    position -=  speed * z;
    lookAt -=  speed * z;
}

void Raytracing::Camera::left()
{
    position -= speed * x;
    lookAt -= speed * x;
}


void Raytracing::Camera::right()
{
    position += speed * x;
    lookAt += speed * x;
}

void Raytracing::Camera::up()
{
    position -= speed * y;
    lookAt -= speed * y;
}

void Raytracing::Camera::down()
{
    position += speed * y;
    lookAt += speed * y;
}

void Raytracing::Camera::lookLeft()
{
    x = Vector3(
        x.m_x * cosRotationSpeed + x.m_z * sinRotationSpeed,
        x.m_y,
        - x.m_x * sinRotationSpeed + x.m_z * cosRotationSpeed
    );
    z = x.Cross(y);
    lookAt = position + z;
}
void Raytracing::Camera::lookRight()
{
    x = Vector3(
        x.m_x * cosRotationSpeed - x.m_z * sinRotationSpeed,
        x.m_y,
        x.m_x * sinRotationSpeed + x.m_z * cosRotationSpeed
    );
    z = x.Cross(y);
    lookAt = position + z;
}

void Raytracing::Camera::rotateClockWise()
{
    upVector = Vector3(
        upVector.m_x * cosRotationSpeed - upVector.m_y * sinRotationSpeed,
        upVector.m_x * sinRotationSpeed + upVector.m_y * cosRotationSpeed,
        upVector.m_z
    );
    computeBase();
}

void Raytracing::Camera::rotateAntiClockWise()
{
    upVector = Vector3(
        upVector.m_x * cosRotationSpeed + upVector.m_y * sinRotationSpeed,
        - upVector.m_x * sinRotationSpeed + upVector.m_y * cosRotationSpeed,
        upVector.m_z
    );
    computeBase();
}

// compute base
void Raytracing::Camera::computeBase()
{

    z = Normalize(lookAt - position);
    x = Normalize(upVector.Cross(z));
    y = z.Cross(x);
    // std::cout << "x = " << x << std::endl;
    // std::cout << "y = " << y << std::endl;
    // std::cout << "z = " << z << std::endl;
}
