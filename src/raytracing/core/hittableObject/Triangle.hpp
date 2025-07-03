# pragma once 

# include "raytracing/utils/vector.hpp"

namespace Raytracing
{
    class Triangle
    {
    private:
        Vector3 vertex1;
        Vector3 vertex2;
        Vector3 vertex3;
    public:
        Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        ~Triangle() = default;
    };

} // namespace Raytracing
