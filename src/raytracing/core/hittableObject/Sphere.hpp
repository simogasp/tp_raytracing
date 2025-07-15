#pragma once

#include "stdint.h"
#include "raytracing/utils/vector.hpp"
#include "raytracing/core/Ray.hpp"
#include "raytracing/core/Camera.hpp"

#define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    struct Sphere
    {
        glm::vec3 center;
        float radius;
        uint32_t materialIndex;

        double intersect(const Camera *camera, const Ray *ray) const
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
            // std::cout << "camera->getNear() <<" "<< dist <<" "<< dist <<" "<< camera->getFar()"<<std::endl;

            if (D < 0.0 || dop < 0)
            {
                return glm::min(D, dop);
            }

            const double sqrtD = sqrt(D);

            const double tmin = dop - sqrtD;
            // check if the point is in the clip region and before the last hit
            glm::vec3 hitpos = ray->origin + (float) tmin * ray->direction;
            float dist = (hitpos - camera->getPosition()).length();
            if (camera->getNear() < dist && dist < camera->getFar())
            {
                // std::cout << camera->getNear() <<" "<< dist <<" "<< dist <<" "<< camera->getFar()<<std::endl;
                return tmin;
            }

            const double tmax = dop + sqrtD;
            hitpos = ray->origin + (float) tmax * ray->direction;
            dist = (hitpos - camera->getPosition()).length();
            if (camera->getNear() < dist && dist < camera->getFar())
            {
                return tmax;
            }
            // the sphere is clipped
            return -1;
        }
    };

} // namespace Raytracing
