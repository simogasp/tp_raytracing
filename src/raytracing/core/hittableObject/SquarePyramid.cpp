#include "SquarePyramid.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <array>
#include <limits>

namespace
{
    constexpr size_t PlaneCount = 5;

    glm::vec3 estimateNormal(const Raytracing::HittableObject& object, const glm::vec3& position)
    {
        constexpr float eps = 1e-4f;
        const float dx = static_cast<float>(object.sdf(position + glm::vec3(eps, 0.f, 0.f)) - object.sdf(position - glm::vec3(eps, 0.f, 0.f)));
        const float dy = static_cast<float>(object.sdf(position + glm::vec3(0.f, eps, 0.f)) - object.sdf(position - glm::vec3(0.f, eps, 0.f)));
        const float dz = static_cast<float>(object.sdf(position + glm::vec3(0.f, 0.f, eps)) - object.sdf(position - glm::vec3(0.f, 0.f, eps)));
        return glm::normalize(glm::vec3(dx, dy, dz));
    }

    glm::vec4 makePlane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
        float offset = glm::dot(normal, a);
        if (offset < 0.f)
        {
            normal = -normal;
            offset = -offset;
        }
        return glm::vec4(normal, offset);
    }

    std::array<glm::vec4, PlaneCount> buildPlanes(const glm::vec2& halfBaseSize, float height)
    {
        const glm::vec3 apex(0.f, height * 0.5f, 0.f);
        const float yBase = -height * 0.5f;
        const glm::vec3 v0(-halfBaseSize.x, yBase, -halfBaseSize.y);
        const glm::vec3 v1(halfBaseSize.x, yBase, -halfBaseSize.y);
        const glm::vec3 v2(halfBaseSize.x, yBase, halfBaseSize.y);
        const glm::vec3 v3(-halfBaseSize.x, yBase, halfBaseSize.y);

        return {
            glm::vec4(0.f, -1.f, 0.f, height * 0.5f),
            makePlane(v0, v1, apex),
            makePlane(v1, v2, apex),
            makePlane(v2, v3, apex),
            makePlane(v3, v0, apex),
        };
    }

    std::array<glm::vec3, 5> buildVertices(const glm::vec2& halfBaseSize, float height)
    {
        const float yBase = -height * 0.5f;
        return {
            glm::vec3(-halfBaseSize.x, yBase, -halfBaseSize.y),
            glm::vec3(halfBaseSize.x, yBase, -halfBaseSize.y),
            glm::vec3(halfBaseSize.x, yBase, halfBaseSize.y),
            glm::vec3(-halfBaseSize.x, yBase, halfBaseSize.y),
            glm::vec3(0.f, height * 0.5f, 0.f),
        };
    }

    Raytracing::AxisAlignedBoundingBox boundingBoxFromVertices(const std::array<glm::vec3, 5>& vertices,
                                                               const glm::mat3& rotationMatrix,
                                                               const glm::vec3& center)
    {
        glm::vec3 minBounds(std::numeric_limits<float>::max());
        glm::vec3 maxBounds(std::numeric_limits<float>::lowest());
        for (const glm::vec3& vertex : vertices)
        {
            const glm::vec3 worldVertex = center + rotationMatrix * vertex;
            minBounds = glm::min(minBounds, worldVertex);
            maxBounds = glm::max(maxBounds, worldVertex);
        }
        return {minBounds, maxBounds};
    }
}

Raytracing::SquarePyramid::SquarePyramid(const glm::vec3& newBaseCenter,
                                         const glm::vec2& newHalfBaseSize,
                                         float newHeight,
                                         const glm::vec3& newRotation,
                                         uint32_t newMaterialIndex)
    : HittableObject(newMaterialIndex)
    , baseCenter(newBaseCenter)
    , halfBaseSize(newHalfBaseSize)
    , height(newHeight)
    , rotation(newRotation)
    , center(newBaseCenter)
    , rotationMatrix(glm::mat3(glm::eulerAngleXYZ(newRotation.x, newRotation.y, newRotation.z)))
    , inverseRotationMatrix(glm::transpose(rotationMatrix))
{
    center = baseCenter + rotationMatrix * glm::vec3(0.f, height * 0.5f, 0.f);
}

double Raytracing::SquarePyramid::sdf(const glm::vec3& position) const
{
    const glm::vec3 localPosition = inverseRotationMatrix * (position - center);
    const std::array<glm::vec4, PlaneCount> planes = buildPlanes(halfBaseSize, height);

    float distance = -std::numeric_limits<float>::max();
    for (const glm::vec4& plane : planes)
        distance = glm::max(distance, glm::dot(glm::vec3(plane), localPosition) - plane.w);

    return distance;
}

glm::vec3 Raytracing::SquarePyramid::getNormal(glm::vec3 position) const
{
    return estimateNormal(*this, position);
}

bool Raytracing::SquarePyramid::hasBoundingBox() const
{
    return true;
}

Raytracing::AxisAlignedBoundingBox Raytracing::SquarePyramid::getBoundingBox() const
{
    return boundingBoxFromVertices(buildVertices(halfBaseSize, height), rotationMatrix, center);
}
