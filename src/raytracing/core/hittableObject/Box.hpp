#pragma once

#include "HittableObject.hpp"
#include <glm/glm.hpp>

namespace Raytracing
{
    struct Box : public HittableObject
    {
        Box(const glm::vec3& newCenter,
            const glm::vec3& newHalfSize,
            const glm::vec3& newRotation,
            uint32_t newMaterialIndex);

        glm::vec3 center;
        glm::vec3 halfSize;
        glm::vec3 rotation;

        double sdf(const glm::vec3& position) const override;

        [[nodiscard]] glm::vec3 getNormal(glm::vec3 position) const override;

        [[nodiscard]] bool hasBoundingBox() const override;

        [[nodiscard]] AxisAlignedBoundingBox getBoundingBox() const override;

    private:
        glm::mat3 rotationMatrix;
        glm::mat3 inverseRotationMatrix;
    };
}
