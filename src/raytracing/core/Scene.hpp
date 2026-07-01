# pragma once
# include "hittableObjects/HittableObject.hpp"
# include "hittableObjects/Material.hpp"
# include <memory>
# include <vector>

namespace Raytracing
{
    class Scene
    {
    public:
        using ObjectPtr = std::shared_ptr<HittableObject>;
        using ObjectList = std::vector<ObjectPtr>;

    private:
        /**
         * The list of objects in the scene.
         */
        ObjectList objectList;
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
         * Gets the list of objects.
         * @return a vector of objects.
         */
        [[nodiscard]] const ObjectList& getListObjects() const;

        /**
         * Gets the (modifiable) list of objects.
         * @return a vector of objects.
         */
        [[nodiscard]] ObjectList& getListObjects();
        
        /**
         * Gets the list of material.
         * @return a vector of Material.
         */
        [[nodiscard]] const std::vector<Raytracing::Material>& getListMaterial() const;

        /**
         * Gets the (modifiable) list of material.
         * @return a vector of Material.
         */
        [[nodiscard]] std::vector<Raytracing::Material>& getListMaterial();

        /**
         * Adds the given object to the scene.
         * @param object the object
         */
        void addObject(ObjectPtr object);

        /**
         * Adds the material to the scene.
         */
        void addMaterial(const Raytracing::Material& material);
    };
    
} // namespace Raytracing
