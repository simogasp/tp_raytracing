#pragma once

#include "HittableObject.hpp"
#include <glm/glm.hpp>

namespace Raytracing
{
    struct TriangularPyramid : public HittableObject
    {
        TriangularPyramid(const glm::vec3& newBaseCenter,
                          float newBaseRadius,
                          float newHeight,
                          const glm::vec3& newRotation,
                          uint32_t newMaterialIndex);

        glm::vec3 baseCenter;
        float baseRadius;
        float height;
        glm::vec3 rotation;

        double sdf(const glm::vec3& position) const override;

        [[nodiscard]] glm::vec3 getNormal(glm::vec3 position) const override;

        [[nodiscard]] bool hasBoundingBox() const override;

        [[nodiscard]] AxisAlignedBoundingBox getBoundingBox() const override;

    private:
        glm::vec3 center;
        glm::mat3 rotationMatrix;
        glm::mat3 inverseRotationMatrix;
    };
}
