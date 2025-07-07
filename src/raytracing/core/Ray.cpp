#include "Ray.hpp"

namespace Raytracing
{

    Ray::Ray(const Vector3 &originRay, const Vector3 &directionRay,
            const float nearRay, const float farRay)
    {
        origin = originRay;
        direction = directionRay;
        color = IM_COL32(0, 0, 0, 255);
        near = nearRay;
        far = farRay;
        depth = far;
    }

    Vector3 Ray::getOrigin()
    {
        return origin;
    }

    Vector3 Ray::getDirection()
    {
        return direction;
    }

    float Ray::getDepth()
    {
        return depth;
    }

    float Ray::getNear()
    {
        return near;
    }

    void
    Ray::setDepth(const float newDepth)
    {
        depth = newDepth;
    }
    
    void Ray::setNear(const float newNear)
    {
        near = newNear;
    }

    void Ray::setFar(const float newFar)
    {
        far = newFar;
    }

    void Ray::reset()
    {
        depth = far;
    }

    void Ray::setDirection(const Vector3 &newDirection)
    {
        direction = newDirection;
    }

    void Ray::setOrigin(const Vector3 &newOrigin)
    {
        origin = newOrigin; 
    }

} // namespace Raytracing