# include "Ray.hpp"
# include <iostream>

namespace Raytracing
{
    Ray::Ray()
    {
        far = 500;
    }

    Ray::Ray(const glm::vec3 &originRay, const glm::vec3 &directionRay,
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

    glm::vec3 Ray::getOrigin()
    {
        return origin;
    }

    glm::vec3 Ray::getDirection()
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

    void Ray::setDirection(const glm::vec3 &newDirection)
    {
        direction = newDirection;
    }

    void Ray::setOrigin(const glm::vec3 &newOrigin)
    {
        origin = newOrigin; 
    }

    void Ray::setColor(ImColor newColor)
    {
        color = newColor;
    }

    void Ray::setNormalHit(const double t, const glm::vec3 &center)
    {
        const glm::vec3 hitPos = origin + direction * glm::vec1(t);
        glm::vec3 normal(hitPos - center);
        normal = glm::normalize(normal);
        // std::cout << "orig = " << origin.x << " " << origin.y << " " 
        // << origin.z << " t = " << t << " dir = " << direction.x << " "
        //  << direction.y << " " << direction.z << " hitpos = " 
        //  << hitPos.x << " " << hitPos.y << " " << hitPos.z <<  " norm = " << normal.x << " " << normal.y << " " << normal.z << std::endl;
        // color = IM_COL32((int) ((normal.m_x + 1) / 2 * 255), (int) ((normal.m_y + 1) / 2 * 255), (int) ((normal.m_z + 1) / 2 * 255), 255);
        // printf("%0x\n", color);
        const double l = glm::max(glm::dot(normal, glm::normalize(glm::vec3(1, 1, 1))), 0.f);
        color = IM_COL32(l * 255, 00, l * 255, 255);
    }

} // namespace Raytracing