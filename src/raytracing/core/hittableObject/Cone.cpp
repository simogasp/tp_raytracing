#include "Cone.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

namespace
{
    float distanceToSegment(const glm::vec2& point, const glm::vec2& start, const glm::vec2& end)
    {
        const glm::vec2 segment = end - start;
        const float projection = glm::clamp(glm::dot(point - start, segment) / glm::dot(segment, segment), 0.f, 1.f);
        return glm::length(point - (start + projection * segment));
    }

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

Raytracing::Cone::Cone(const glm::vec3& newBaseCenter,
                       float newRadius,
                       float newHeight,
                       const glm::vec3& newRotation,
                       uint32_t newMaterialIndex)
    : HittableObject(newMaterialIndex)
    , baseCenter(newBaseCenter)
    , radius(newRadius)
    , height(newHeight)
    , rotation(newRotation)
    , center(newBaseCenter)
    , rotationMatrix(glm::mat3(glm::eulerAngleXYZ(newRotation.x, newRotation.y, newRotation.z)))
    , inverseRotationMatrix(glm::transpose(rotationMatrix))
{
    center = baseCenter + rotationMatrix * glm::vec3(0.f, height * 0.5f, 0.f);
}

double Raytracing::Cone::sdf(const glm::vec3& position) const
{
    const glm::vec3 localCenteredPosition = inverseRotationMatrix * (position - center);
    const glm::vec3 localPosition = localCenteredPosition + glm::vec3(0.f, height * 0.5f, 0.f);
    const glm::vec2 profile(glm::length(glm::vec2(localPosition.x, localPosition.z)), localPosition.y);

    const glm::vec2 baseLeft(0.f, 0.f);
    const glm::vec2 baseRight(radius, 0.f);
    const glm::vec2 tip(0.f, height);

    const float sideDistance = distanceToSegment(profile, baseRight, tip);
    const float baseDistance = distanceToSegment(profile, baseLeft, baseRight);
    const float distance = glm::min(sideDistance, baseDistance);

    const float radiusAtHeight = radius * (1.f - glm::clamp(localPosition.y / height, 0.f, 1.f));
    const bool inside = localPosition.y >= 0.f && localPosition.y <= height && profile.x <= radiusAtHeight;
    return inside ? -distance : distance;
}

glm::vec3 Raytracing::Cone::getNormal(glm::vec3 position) const
{
    return estimateNormal(*this, position);
}

bool Raytracing::Cone::hasBoundingBox() const
{
    return true;
}

Raytracing::AxisAlignedBoundingBox Raytracing::Cone::getBoundingBox() const
{
    const glm::vec3 extent = orientedExtent(rotationMatrix, glm::vec3(radius, height * 0.5f, radius));
    return {center - extent, center + extent};
}
