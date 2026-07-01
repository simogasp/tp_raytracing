
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace Raytracing
{
struct AxisAlignedBoundingBox
{
    glm::vec3 min;
    glm::vec3 max;
};

// Abstract class to represent ray-hittable objects
class HittableObject {
   public:
        explicit HittableObject(uint32_t newMaterialIndex)
            : materialIndex(newMaterialIndex)
        {
        }

        virtual ~HittableObject() = default;

        // pure virtual function providing interface framework.
        
        /**
         * Returns the SIGNED distance from a given position
         * to the nearest point on the surface of the object
         * 
         * The distance is 'signed' such that it is :
         *     - positive if the point is outside of the object's volume, 
         *     - zero if point is on the surface of the object's volume,
         *     - negative if the point is inside the object's volume.)
         * 
         * @return the (signed) distance
         */
        virtual double sdf(const glm::vec3& position) const = 0;

        /**
         * Returns the normal for a position around the object.
         * 
         * @return the normal
         */
        [[nodiscard]] virtual glm::vec3 getNormal(glm::vec3 position) const = 0;

        /**
         * Whether this object has a finite axis-aligned bounding box.
         * Objects without one are always sampled during scene SDF queries.
         */
        [[nodiscard]] virtual bool hasBoundingBox() const
        {
            return false;
        }

        /**
         * Returns the finite axis-aligned bounding box for this object.
         * Only valid when hasBoundingBox() returns true.
         */
        [[nodiscard]] virtual AxisAlignedBoundingBox getBoundingBox() const
        {
            return {};
        }

        [[nodiscard]] uint32_t getMaterialIndex() const
        {
            return materialIndex;
        }

    protected:
        // The material Index.
        uint32_t materialIndex;
};

} // namespace Raytracing
