#pragma once

#include "HittableObject.hpp"
#include <memory>

namespace Raytracing
{
    enum class CsgOperation
    {
        Union,
        Subtraction,
        Intersection
    };

    struct CsgObject : public HittableObject
    {
        CsgObject(std::shared_ptr<HittableObject> newLeft,
                  std::shared_ptr<HittableObject> newRight,
                  CsgOperation newOperation,
                  uint32_t newMaterialIndex);

        std::shared_ptr<HittableObject> left;
        std::shared_ptr<HittableObject> right;
        CsgOperation operation;

        double sdf(const glm::vec3& position) const override;

        [[nodiscard]] glm::vec3 getNormal(glm::vec3 position) const override;

        [[nodiscard]] bool hasBoundingBox() const override;

        [[nodiscard]] AxisAlignedBoundingBox getBoundingBox() const override;
    };
}
