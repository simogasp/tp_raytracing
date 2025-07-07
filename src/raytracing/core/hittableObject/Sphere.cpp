#include "Sphere.hpp"

Raytracing::Sphere::Sphere(const float theRadius, const Vector3 &pos)
{
    radius = theRadius;
    center = pos;
}

bool Raytracing::Sphere::intersect(Ray * ray)
{
    // printf("ray = %f %f %f\n", ray->getOrigin().m_x, ray->getOrigin().m_y, ray->getOrigin().m_z);
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

    const Vector3 op = center - ray->getOrigin();
    const double dop = ray->getDirection().Dot(op);
    const double D = dop * dop - op.Dot(op) + radius * radius;

    if (D < 0.0)
    {
        return false;
    }
    
    const double sqrtD = sqrt(D);
    
    const double tmin = dop - sqrtD;
    if (ray->getNear() < tmin && tmin < ray->getDepth())
    {
        ray->setDepth(tmin);
        return true;
    }

    const double tmax = dop + sqrtD;
    if (ray->getNear() < tmax && tmax < ray->getDepth())
    {
        ray->setDepth(tmax);
        return true;
    }

    return false;
}