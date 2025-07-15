# pragma once
# include "hittableObject/Sphere.hpp"
# include "hittableObject/Material.hpp"
# include "hittableObject/Triangle.hpp"
# include <vector>

namespace Raytracing
{
    class Scene
    {
    private:
        std::vector<Raytracing::Sphere> sphereList;
        // std::vector<Raytracing::Triangle> triangleList = std::vector<Raytracing::Triangle>();
        std::vector<Raytracing::Material> materialList;
    public:
        Scene() = default;
        ~Scene() = default;

        std::vector<Raytracing::Sphere> getListSphere() const;
        std::vector<Raytracing::Material> getListMaterial() const;
        // std::vector<Raytracing::Triangle> getListTriangle();
        void addSphere(const Raytracing::Sphere& sphere);
        void addMaterial(const Raytracing::Material& material);
        void addRandomSphere();

    };
    
} // namespace Raytracing
