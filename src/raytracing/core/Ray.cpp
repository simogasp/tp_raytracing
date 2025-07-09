#include "Ray.hpp"

namespace Raytracing
{
    Ray::Ray()
    {
        far = 500;
    }

    Ray::Ray(const Vector3 &originRay, const Vector3 &directionRay,
            const float nearRay, const float farRay)
    {
        origin = originRay;
        direction = directionRay;
        color = IM_COL32(0, 0, 0, 255);
        near = nearRay;
        far = farRay;
        depth = far;
        color = IM_COL32(0, 0, 0, 0);
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

    ImColor Ray::getColor()
    {
        return color;
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

    void Ray::setColor(ImColor newColor)
    {
        color = newColor;
    }

    void Ray::setNormalHit(const double t, const Vector3 &center)
    {
        const Vector3 hitPos(origin + t * direction);
        Vector3 normal(hitPos - center);
        normal = Normalize(normal);
        // std::cout << "orig = " << origin << " t = " << t << " dir = " << direction << " hitpos = " << hitPos << " norm = " << normal << std::endl;
        // color = IM_COL32((int) ((normal.m_x + 1) / 2 * 255), (int) ((normal.m_y + 1) / 2 * 255), (int) ((normal.m_z + 1) / 2 * 255), 255);
        // printf("%0x\n", color);
        const double l = std::max(normal.Dot(Normalize(Vector3(1, 1, 1))), 0.);
        color = IM_COL32(l * 255, 00, l * 255, 255);
    }

} // namespace Raytracing