#include "Sphere.hpp"
# define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
# include <glm/gtx/norm.hpp>



double Raytracing::Sphere::intersect(const Camera *camera, const Ray *ray) const
{
    // (o + t*d - p) . (o + t*d - p) - r*r = 0
    // <=> (d . d) * t^2 + 2 * d . (o - p) * t + (o - p) . (o - p) - r*r = 0
    //
    // Discriminant check
    // (2 * d . (o - p))^2 - 4 * (d . d) * ((o - p) . (o - p) - r*r) <? 0
    // <=> (d . (o - p))^2 - (d . d) * ((o - p) . (o - p) - r*r) <? 0
    // <=> (d . op)^2 - 1 * (op . op - r*r) <? 0
    // <=> b^2 - (op . op) + r*r <? 0
    // <=> D <? 0
    //
    // Solutions
    // t = (- 2 * d . (o - p) +- 2 * sqrt(D)) / (2 * (d . d))
    // <=> t = dop +- sqrt(D)
    const glm::vec3 op = center - ray->origin;
    const double dop = glm::dot(ray->direction, op);
    const double D = dop * dop - glm::dot(op, op) + radius * radius;
    // if (glm :: dot(glm::normalize(ray->direction), glm :: normalize(camera->getLookAt() - camera->getPosition())) > 0.99999999 && ray->bounce < 1)
    // {
    //     std :: cout << "-----------------------------------" << std :: endl;
    //     std :: cout << "ray o = " << glm::to_string(ray->origin) << std :: endl;
    //     std :: cout << "ray d = " << glm::to_string(ray->direction) << std :: endl;
    //     std :: cout << "p = " << glm::to_string(center) << std :: endl;
    //     std :: cout << "r = " << radius << std :: endl;
    //     std :: cout << "op = " << glm :: to_string(op) << std::endl;
    //     std :: cout << "dop2 = " << dop * dop << std::endl;
    //     std :: cout << "||op||2 = " << glm::dot(op, op) << std::endl;
    //     std :: cout << "r2 = " << radius * radius << std::endl;

    //     std :: cout << "dop = " << dop << std :: endl;
    //     std :: cout << "D = " << D << std::endl;
    //     std :: cout << "-----------------------------------" << std :: endl;
    // }
    
    if (D < 0.0)
    {
        return glm::min(D, dop);
    }

    
    const double sqrtD = sqrt(D);
    
    const double tmin = dop - sqrtD;
    // check if the point is in the clip region and before the last hit
    glm::vec3 hitpos = ray->origin + (float)tmin * ray->direction;
    glm::vec3 distVect = hitpos - camera->getPosition();

    float dist = glm::sqrt(glm::dot(distVect, distVect));
    if (tmin > 0 && camera->getNear() < dist && dist < camera->getFar())
    {
        return tmin;
    }
    
    const double tmax = dop + sqrtD;
    hitpos = ray->origin + (float)tmax * ray->direction;
    distVect = hitpos - camera->getPosition();
    dist = glm::sqrt(glm::dot(distVect, distVect));
    if (tmax > 0 && camera->getNear() < dist && dist < camera->getFar())
    {
        return tmax;
    }
    // the sphere is clipped
    return -1;
}
