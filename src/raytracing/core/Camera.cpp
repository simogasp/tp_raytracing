#include "Camera.hpp"
#include "Ray.hpp"
#include "raytracing/utils/math.hpp"

Raytracing::Camera::Camera(const glm::vec3 &positionCamera, const glm::vec3 &lookAtCamera,
                           const glm::vec3 &upCamera, const float fieldOfViewCamera,
                           const float nearCamera, const float farCamera)
{
    position = positionCamera;
    lookAt = lookAtCamera;
    upVector = upCamera;
    fieldOfView = std::clamp((double)fieldOfViewCamera, 0.0, 2.0 * g_pi);
    near = nearCamera;
    far = farCamera;

    computeBase();
}

Raytracing::Camera::~Camera()
{
}

glm::vec3 Raytracing::Camera::getPosition()
{
    return position;
}

glm::vec3 Raytracing::Camera::getLookAt()
{
    return lookAt;
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

std::vector<glm::vec3> Raytracing::Camera::getRayDirections()
{
    return rayDirections;
}

glm::vec3 Raytracing::Camera::baseChangment(glm::vec3 vect)
{
    // std ::cout << "x " << x.x << " " << x.y << " " << x.z << std::endl;
    // std ::cout << "y " << y.x << " " << y.y << " " << y.z << std::endl;
    // std ::cout << "z " << z.x << " " << z.y << " " << z.z << std::endl;
    // std ::cout << vect.x << " " << vect.y << " " << vect.z << std::endl;
    glm::mat3 mat(x, y, z);
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            // std::cout << mat[i][j] << " " << std::ends;
        }
        // std ::cout << std::endl;
    }

    glm::vec3 res(glm::mat3(x, y, z) * vect);
    // std ::cout << res.x << " " << res.y << " " << res.z << std::endl;
    return res;
}

void Raytracing::Camera::setFov(const double newValue)
{
    fieldOfView = newValue;
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
    near = newNear;
}

void Raytracing::Camera::setFar(const double newFar)
{
    far = newFar;
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
    glm::mat3 rot(
        cosRotationSpeed, 0, sinRotationSpeed,
        0, 1, 0,
        -sinRotationSpeed, 0, cosRotationSpeed);
    x = rot * x;
    z = rot * z;
    lookAt = position + z;
}
void Raytracing::Camera::lookRight()
{
    glm::mat3 rot(
        cosRotationSpeed, 0, -sinRotationSpeed,
        0, 1, 0,
        sinRotationSpeed, 0, cosRotationSpeed);
    x = rot * x;
    z = rot * z;
    lookAt = position + z;
}

void Raytracing::Camera::rotateClockWise()
{
    glm::mat3 rot(
        cosRotationSpeed, -sinRotationSpeed, 0,
        sinRotationSpeed, cosRotationSpeed, 0,
        0, 0, 1);
    upVector = rot * upVector;
    computeBase();
}

void Raytracing::Camera::rotateAntiClockWise()
{
    glm::mat3 rot(
        cosRotationSpeed, sinRotationSpeed, 0,
        -sinRotationSpeed, cosRotationSpeed, 0,
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
    const float invScreenRatio = (float)height / width;

    uint32_t x, y;
    // #pragma omp parallel for collapse(2)
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            // relative placement on the screen of the pixel
            const float relativeX = 2 * (float)x / (width - 1) - 1;
            const float relativeY = 2 * (float)y / (height - 1) - 1;

            // rotation angle
            const float fovOn2 = fieldOfView / 2;

            const float theta = fovOn2 * relativeX;
            const float phi = fovOn2 * relativeY * invScreenRatio;

            const float sintheta = sin(theta);
            const float costheta = cos(theta);

            const float sinphi = sin(phi);
            const float cosphi = cos(phi);

            // point on the screen is also

            rayDirections[x + y * width] = baseChangment(glm::vec3(
                cosphi * sintheta, // y nor
                sinphi,            // z nor
                cosphi * costheta  // x nor
                ));
            ;
            if (x % (width / 3) == 0 && y % (height / 3) == 0)
            {
                // std::cout << rayDirections[x + y * width].x << " " << " " << rayDirections[x + y * width].y << " " << rayDirections[x + y * width].z << std::endl;
            }
        }
    }
}

// compute base
void Raytracing::Camera::computeBase()
{
    z = glm::normalize(lookAt - position);
    x = glm::normalize(glm::cross(upVector, z));
    y = glm::cross(z, x);
    // std::cout << "c x = " << x.x << " " << x.y << " " << x.z << std::endl;
    // std::cout << "c y = " << y.x << " " << y.y << " " << y.z << std::endl;
    // std::cout << "c z = " << z.x << " " << z.y << " " << z.z << std::endl;

    updateRay();
}
