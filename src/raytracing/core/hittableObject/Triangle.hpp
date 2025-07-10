# pragma once 

# include "glm/glm.hpp"

namespace Raytracing
{
    class Triangle
    {
    private:
        glm::vec3 vertex1;
        glm::vec3 vertex2;
        glm::vec3 vertex3;
    public:
        Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
        ~Triangle() = default;
    };

} // namespace Raytracing
