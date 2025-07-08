# pragma once

# include "imgui.h"
# include "raytracing/utils/vector.hpp"

namespace Raytracing
{
    class Ray
    {
    private:
        Vector3 origin;
        Vector3 direction;
        ImColor color;
        float near;
        float far;
        float depth;
    public:
        Ray();
        Ray(const Vector3& origin, const Vector3& direction,
            const float nearRay, const float farRay);
        ~Ray() = default;

        [[ nodiscard ]]
        Vector3 getOrigin();
        
        [[ nodiscard ]]
        Vector3 getDirection();
        
        [[ nodiscard ]]
        float getNear();
        
        [[ nodiscard ]]
        float getDepth();

        [[ nodiscard ]]
        ImColor getColor();

        void setDepth(const float newDepth);

        void setNear(const float newNear);

        void setFar(const float newFar);

        
        void reset();
        
        void setDirection(const Vector3& newDirection);
        
        void setOrigin(const Vector3 &newOrigin);

        void setColor(ImColor newColor);

        void setNormalHit(const double t, const Vector3& center);
    };
    
    
} // namespace Raytracing
