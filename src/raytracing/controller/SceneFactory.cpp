#include "SceneFactory.hpp"

#include "raytracing/gui/App.hpp"

#include <algorithm>
#include <memory>

Raytracing::SceneFactory::SceneFactory()
{
    currentScene = Scene();
}

void Raytracing::SceneFactory::createNewScene()
{
    currentScene = Scene();
}

void Raytracing::SceneFactory::pushSphere(const glm::vec3 center, float radius, unsigned int materialIndex)
{
    currentScene.addObject(std::make_shared<Sphere>(center, radius, materialIndex));
}

void Raytracing::SceneFactory::pushBox(const glm::vec3 center, const glm::vec3 halfSize, const glm::vec3 rotation, unsigned int materialIndex)
{
    currentScene.addObject(std::make_shared<Box>(center, halfSize, rotation, materialIndex));
}

void Raytracing::SceneFactory::pushCylinder(const glm::vec3 baseCenter, float radius, float height, const glm::vec3 rotation, unsigned int materialIndex)
{
    currentScene.addObject(std::make_shared<Cylinder>(baseCenter, radius, height, rotation, materialIndex));
}

void Raytracing::SceneFactory::pushCone(const glm::vec3 baseCenter, float radius, float height, const glm::vec3 rotation, unsigned int materialIndex)
{
    currentScene.addObject(std::make_shared<Cone>(baseCenter, radius, height, rotation, materialIndex));
}

void Raytracing::SceneFactory::pushMaterial(const glm::vec3& reflectionColor, float shininess, float roughness)
{
    Material mat;
    mat.reflection = reflectionColor;
    mat.shinyness = shininess;
    mat.roughness = roughness;
    currentScene.addMaterial(mat);
}

void Raytracing::SceneFactory::pushMaterial(const glm::vec3& reflectionColor,
                                            const glm::vec3& emissionColor,
                                            float shininess,
                                            float roughness,
                                            float emissionPower,
                                            float attenuationRadius,
                                            float refractionIndex,
                                            float translucentAbsorption)
{
    Material mat;
    mat.emission = emissionColor;
    mat.reflection = reflectionColor;
    mat.shinyness = shininess;
    mat.roughness = roughness;
    mat.emissionPower = emissionPower;
    mat.attenuationRadius = attenuationRadius;
    mat.refractionIndex = refractionIndex;
    mat.translucentAbsorption = translucentAbsorption;
    currentScene.addMaterial(mat);
}

void Raytracing::SceneFactory::pushMaterial(float refractionIndex)
{
    Material mat;
    mat.shinyness = 1.f;
    mat.roughness = 0.f;
    mat.refractionIndex = refractionIndex;
    currentScene.addMaterial(mat);
}

void Raytracing::SceneFactory::pushMaterial(float refractionIndex, float roughness)
{
    Material mat;
    mat.shinyness = 1.f;
    mat.roughness = roughness;
    mat.refractionIndex = refractionIndex;
    currentScene.addMaterial(mat);
}

void Raytracing::SceneFactory::pushMaterial(float refractionIndex, float roughness, float translucentAbsorption)
{
    Material mat;
    mat.shinyness = 1.f;
    mat.roughness = roughness;
    mat.refractionIndex = refractionIndex;
    mat.translucentAbsorption = translucentAbsorption;
    currentScene.addMaterial(mat);
}

void Raytracing::SceneFactory::popSphere()
{
    auto& objects = currentScene.getListObjects();
    const auto sphere = std::find_if(objects.rbegin(), objects.rend(), [](const Scene::ObjectPtr& object) {
        return std::dynamic_pointer_cast<Sphere>(object) != nullptr;
    });

    if (sphere != objects.rend())
        objects.erase(std::next(sphere).base());
}

const Raytracing::Scene& Raytracing::SceneFactory::getScene() const
{
    return currentScene;
}

Raytracing::Scene Raytracing::SceneFactory::getDefaultScene()
{
    Scene res = Scene();
    return res;
}

std::vector<Raytracing::Sphere> Raytracing::SceneFactory::getListOfSphere()
{
    std::vector<Sphere> spheres;
    for (const Scene::ObjectPtr& object : currentScene.getListObjects())
    {
        if (const auto sphere = std::dynamic_pointer_cast<Sphere>(object))
            spheres.push_back(*sphere);
    }
    return spheres;
}
