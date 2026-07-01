#include "Box.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

namespace
{
    glm::vec3 estimateNormal(const Raytracing::HittableObject& object, const glm::vec3& position)
    {
        constexpr float eps = 1e-4f;
        const float dx = static_cast<float>(object.sdf(position + glm::vec3(eps, 0.f, 0.f)) - object.sdf(position - glm::vec3(eps, 0.f, 0.f)));
        const float dy = static_cast<float>(object.sdf(position + glm::vec3(0.f, eps, 0.f)) - object.sdf(position - glm::vec3(0.f, eps, 0.f)));
        const float dz = static_cast<float>(object.sdf(position + glm::vec3(0.f, 0.f, eps)) - object.sdf(position - glm::vec3(0.f, 0.f, eps)));
        return glm::normalize(glm::vec3(dx, dy, dz));
    }

    glm::vec3 orientedExtent(const glm::mat3& rotationMatrix, const glm::vec3& localExtent)
    {
        const glm::mat3 absRotation(glm::abs(rotationMatrix[0]), glm::abs(rotationMatrix[1]), glm::abs(rotationMatrix[2]));
        return absRotation * localExtent;
    }
}

Raytracing::Box::Box(const glm::vec3& newCenter,
                     const glm::vec3& newHalfSize,
                     const glm::vec3& newRotation,
                     uint32_t newMaterialIndex)
    : HittableObject(newMaterialIndex)
    , center(newCenter)
    , halfSize(newHalfSize)
    , rotation(newRotation)
    , rotationMatrix(glm::mat3(glm::eulerAngleXYZ(newRotation.x, newRotation.y, newRotation.z)))
    , inverseRotationMatrix(glm::transpose(rotationMatrix))
{
}

double Raytracing::Box::sdf(const glm::vec3& position) const
{
    const glm::vec3 localPosition = inverseRotationMatrix * (position - center);
    const glm::vec3 q = glm::abs(localPosition) - halfSize;
    return glm::length(glm::max(q, glm::vec3(0.f))) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);
}

glm::vec3 Raytracing::Box::getNormal(glm::vec3 position) const
{
    return estimateNormal(*this, position);
}

bool Raytracing::Box::hasBoundingBox() const
{
    return true;
}

Raytracing::AxisAlignedBoundingBox Raytracing::Box::getBoundingBox() const
{
    const glm::vec3 extent = orientedExtent(rotationMatrix, halfSize);
    return {center - extent, center + extent};
}
