#include "Icosahedron.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <array>
#include <limits>

namespace
{
    constexpr size_t VertexCount = 12;
    constexpr size_t PlaneCount = 20;

    glm::vec3 estimateNormal(const Raytracing::HittableObject& object, const glm::vec3& position)
    {
        constexpr float eps = 1e-4f;
        const float dx = static_cast<float>(object.sdf(position + glm::vec3(eps, 0.f, 0.f)) - object.sdf(position - glm::vec3(eps, 0.f, 0.f)));
        const float dy = static_cast<float>(object.sdf(position + glm::vec3(0.f, eps, 0.f)) - object.sdf(position - glm::vec3(0.f, eps, 0.f)));
        const float dz = static_cast<float>(object.sdf(position + glm::vec3(0.f, 0.f, eps)) - object.sdf(position - glm::vec3(0.f, 0.f, eps)));
        return glm::normalize(glm::vec3(dx, dy, dz));
    }

    std::array<glm::vec3, VertexCount> buildVertices(float radius)
    {
        constexpr float phi = 1.6180339887498948482f;
        const float scale = radius / glm::length(glm::vec3(0.f, 1.f, phi));

        return {
            scale * glm::vec3(-1.f, phi, 0.f),
            scale * glm::vec3(1.f, phi, 0.f),
            scale * glm::vec3(-1.f, -phi, 0.f),
            scale * glm::vec3(1.f, -phi, 0.f),
            scale * glm::vec3(0.f, -1.f, phi),
            scale * glm::vec3(0.f, 1.f, phi),
            scale * glm::vec3(0.f, -1.f, -phi),
            scale * glm::vec3(0.f, 1.f, -phi),
            scale * glm::vec3(phi, 0.f, -1.f),
            scale * glm::vec3(phi, 0.f, 1.f),
            scale * glm::vec3(-phi, 0.f, -1.f),
            scale * glm::vec3(-phi, 0.f, 1.f),
        };
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

    std::array<glm::vec4, PlaneCount> buildPlanes(float radius)
    {
        const std::array<glm::vec3, VertexCount> vertices = buildVertices(radius);
        return {
            makePlane(vertices[0], vertices[11], vertices[5]),
            makePlane(vertices[0], vertices[5], vertices[1]),
            makePlane(vertices[0], vertices[1], vertices[7]),
            makePlane(vertices[0], vertices[7], vertices[10]),
            makePlane(vertices[0], vertices[10], vertices[11]),
            makePlane(vertices[1], vertices[5], vertices[9]),
            makePlane(vertices[5], vertices[11], vertices[4]),
            makePlane(vertices[11], vertices[10], vertices[2]),
            makePlane(vertices[10], vertices[7], vertices[6]),
            makePlane(vertices[7], vertices[1], vertices[8]),
            makePlane(vertices[3], vertices[9], vertices[4]),
            makePlane(vertices[3], vertices[4], vertices[2]),
            makePlane(vertices[3], vertices[2], vertices[6]),
            makePlane(vertices[3], vertices[6], vertices[8]),
            makePlane(vertices[3], vertices[8], vertices[9]),
            makePlane(vertices[4], vertices[9], vertices[5]),
            makePlane(vertices[2], vertices[4], vertices[11]),
            makePlane(vertices[6], vertices[2], vertices[10]),
            makePlane(vertices[8], vertices[6], vertices[7]),
            makePlane(vertices[9], vertices[8], vertices[1]),
        };
    }

    Raytracing::AxisAlignedBoundingBox boundingBoxFromVertices(const std::array<glm::vec3, VertexCount>& vertices,
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

Raytracing::Icosahedron::Icosahedron(const glm::vec3& newCenter,
                                     float newRadius,
                                     const glm::vec3& newRotation,
                                     uint32_t newMaterialIndex)
    : HittableObject(newMaterialIndex)
    , center(newCenter)
    , radius(newRadius)
    , rotation(newRotation)
    , rotationMatrix(glm::mat3(glm::eulerAngleXYZ(newRotation.x, newRotation.y, newRotation.z)))
    , inverseRotationMatrix(glm::transpose(rotationMatrix))
{
}

double Raytracing::Icosahedron::sdf(const glm::vec3& position) const
{
    const glm::vec3 localPosition = inverseRotationMatrix * (position - center);
    const std::array<glm::vec4, PlaneCount> planes = buildPlanes(radius);

    float distance = -std::numeric_limits<float>::max();
    for (const glm::vec4& plane : planes)
        distance = glm::max(distance, glm::dot(glm::vec3(plane), localPosition) - plane.w);

    return distance;
}

glm::vec3 Raytracing::Icosahedron::getNormal(glm::vec3 position) const
{
    return estimateNormal(*this, position);
}

bool Raytracing::Icosahedron::hasBoundingBox() const
{
    return true;
}

Raytracing::AxisAlignedBoundingBox Raytracing::Icosahedron::getBoundingBox() const
{
    return boundingBoxFromVertices(buildVertices(radius), rotationMatrix, center);
}
