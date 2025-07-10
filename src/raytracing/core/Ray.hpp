# pragma once

# include "imgui.h"
# include "glm/glm.hpp"

namespace Raytracing
{
    class Ray
    {
    private:
        glm::vec3 origin;
        glm::vec3 direction;
        ImColor color;
        float near;
        float far;
        float depth;
    public:
        Ray();
        Ray(const glm::vec3& origin, const glm::vec3& direction,
            const float nearRay, const float farRay);
        ~Ray() = default;

        [[ nodiscard ]]
        glm::vec3 getOrigin();
        
        [[ nodiscard ]]
        glm::vec3 getDirection();
        
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
        
        void setDirection(const glm::vec3& newDirection);
        
        void setOrigin(const glm::vec3 &newOrigin);

        void setColor(ImColor newColor);

        void setNormalHit(const double t, const glm::vec3& center);
    };
    
    
} // namespace Raytracing
