# pragma once
# include "hittableObject/Sphere.hpp"
# include "hittableObject/Triangle.hpp"
# include <vector>

namespace Raytracing
{
    class Scene
    {
    private:
        std::vector<Raytracing::Sphere> sphereList = std::vector<Raytracing::Sphere>();
        std::vector<Raytracing::Triangle> triangleList = std::vector<Raytracing::Triangle>();
    public:
        Scene() = default;
        ~Scene() = default;

        std::vector<Raytracing::Sphere> getListSphere();
        std::vector<Raytracing::Triangle> getListTriangle();
        void addSphere(Raytracing::Sphere sphere);
        void addRandomSphere();

    };
    
} // namespace Raytracing
