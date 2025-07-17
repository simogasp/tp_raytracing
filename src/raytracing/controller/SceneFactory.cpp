#include "SceneFactory.hpp"

Raytracing::SceneFactory::SceneFactory()
{
    currentScene = Scene();
}

void Raytracing::SceneFactory::createNewScene()
{
    currentScene = Scene();
}

void Raytracing::SceneFactory::addRandomSphereToScene()
{
    currentScene.addRandomSphere();
}

void Raytracing::SceneFactory::pushSphere(const glm::vec3 center, const float radius, const uint materialIndex)
{
    currentScene.addSphere({center, radius, materialIndex});
}

void Raytracing::SceneFactory::pushMaterial(const glm::vec3 reflectionColor, const float shinyness, const float roughness)
{
    Material mat;
    mat.reflection = reflectionColor;
    mat.shinyness = shinyness;
    mat.roughness = roughness;
    currentScene.addMaterial(mat);
}

void Raytracing::SceneFactory::pushMaterial(const glm::vec3 reflectionColor,
                                            const glm::vec3 emissionColor,
                                            const float shinyness,
                                            const float roughness,
                                            const float emissionPower,
                                            const float attenuationRadius)
{
    Material mat;
    mat.emission = emissionColor;
    mat.reflection = reflectionColor;
    mat.shinyness = shinyness;
    mat.roughness = roughness;
    mat.emissionPower = emissionPower;
    mat.attenuationRadius = attenuationRadius;
    currentScene.addMaterial(mat);
}

void Raytracing::SceneFactory::popSphere()
{
    currentScene.getListSphere().pop_back();
}

Raytracing::Scene Raytracing::SceneFactory::getScene()
{
    return currentScene;
}

Raytracing::Scene Raytracing::SceneFactory::getDefaultScene()
{
    Scene res = Scene();
    res.addRandomSphere();
    return res;
}

std::vector<Raytracing::Sphere> Raytracing::SceneFactory::getListOfSphere()
{
    return currentScene.getListSphere();
}
