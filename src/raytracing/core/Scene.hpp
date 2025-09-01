# pragma once
# include "hittableObject/Sphere.hpp"
# include "hittableObject/Material.hpp"
# include <vector>

namespace Raytracing
{
    class Scene
    {
    private:
        /**
         * The list of sphere in the scene.
         */
        std::vector<Raytracing::Sphere> sphereList;
        /**
         * The list of material of the scene.
         */
        std::vector<Raytracing::Material> materialList;
    public:
        /**
         * Creates a scene.
         */
        Scene() = default;
        /**
         * Destroys a Scene.
         */
        ~Scene() = default;

        /**
         * Gets the list of sphere.
         * @return a vector of sphere.
         */
        [[nodiscard]] std::vector<Raytracing::Sphere> getListSphere() const;
        /**
         * Gets the list of material.
         * @return a vector of Material.
         */
        [[nodiscard]] std::vector<Raytracing::Material> getListMaterial() const;

        /**
         * Adds the given sphere to the scene.
         * @param sphere the sphere
         */
        void addSphere(const Raytracing::Sphere& sphere);

        /**
         * Adds the material to the scene.
         */
        void addMaterial(const Raytracing::Material& material);
        
        /**
         * Adds a random sphere in a range of 10 around origin and with a random radius below 3 and a random material.
         */
        void addRandomSphere();

    };
    
} // namespace Raytracing
