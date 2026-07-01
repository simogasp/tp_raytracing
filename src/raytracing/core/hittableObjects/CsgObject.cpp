#include "CsgObject.hpp"
#include <algorithm>
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

    Raytracing::AxisAlignedBoundingBox unionBoundingBox(const Raytracing::AxisAlignedBoundingBox& left,
                                                       const Raytracing::AxisAlignedBoundingBox& right)
    {
        return {glm::min(left.min, right.min), glm::max(left.max, right.max)};
    }

    Raytracing::AxisAlignedBoundingBox intersectionBoundingBox(const Raytracing::AxisAlignedBoundingBox& left,
                                                              const Raytracing::AxisAlignedBoundingBox& right)
    {
        return {glm::max(left.min, right.min), glm::min(left.max, right.max)};
    }
}

Raytracing::CsgObject::CsgObject(std::shared_ptr<HittableObject> newLeft,
                                 std::shared_ptr<HittableObject> newRight,
                                 CsgOperation newOperation,
                                 uint32_t newMaterialIndex)
    : HittableObject(newMaterialIndex)
    , left(std::move(newLeft))
    , right(std::move(newRight))
    , operation(newOperation)
{
}

double Raytracing::CsgObject::sdf(const glm::vec3& position) const
{
    const float leftDistance = static_cast<float>(left->sdf(position));
    const float rightDistance = static_cast<float>(right->sdf(position));

    switch (operation)
    {
    case CsgOperation::Union:
        return glm::min(leftDistance, rightDistance);
    case CsgOperation::Subtraction:
        return glm::max(leftDistance, -rightDistance);
    case CsgOperation::Intersection:
        return glm::max(leftDistance, rightDistance);
    }

    return leftDistance;
}

glm::vec3 Raytracing::CsgObject::getNormal(glm::vec3 position) const
{
    return estimateNormal(*this, position);
}

bool Raytracing::CsgObject::hasBoundingBox() const
{
    if (operation == CsgOperation::Subtraction)
        return left->hasBoundingBox();

    return left->hasBoundingBox() && right->hasBoundingBox();
}

Raytracing::AxisAlignedBoundingBox Raytracing::CsgObject::getBoundingBox() const
{
    if (operation == CsgOperation::Subtraction)
        return left->getBoundingBox();

    const AxisAlignedBoundingBox leftBoundingBox = left->getBoundingBox();
    const AxisAlignedBoundingBox rightBoundingBox = right->getBoundingBox();

    if (operation == CsgOperation::Intersection)
        return intersectionBoundingBox(leftBoundingBox, rightBoundingBox);

    return unionBoundingBox(leftBoundingBox, rightBoundingBox);
}
