#include "Sphere.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>



double Raytracing::Sphere::sdf(const glm::vec3& position) const
{
    //++ // TODO : return the (signed) distance from the given position
    //++ //        to the *surface* of the sphere
    return (glm::length(position - center) - radius); //!!
    //++ return 1.0;
}

glm::vec3 Raytracing::Sphere::getNormal(const glm::vec3 position) const
{
    return glm::normalize(position - center); //!!
    //++ // TODO : according to the given position, return the normal
    //++ return glm::vec3(0.f);
}
