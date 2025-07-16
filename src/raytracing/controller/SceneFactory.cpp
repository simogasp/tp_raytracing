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

void Raytracing::SceneFactory::pushMaterial(const glm::vec3 reflectionColor, const glm::vec3 emissionColor, const float shinyness, const float roughness, const float emissionPower)
{
    std::cout<<" matInit1 = " << reflectionColor.r << " " << reflectionColor.g << " " << reflectionColor.b << std::endl;
    std::cout<<" matInit2 = " << emissionColor.r << " " << emissionColor.g << " " << emissionColor.b << " " << emissionPower << std::endl;
    Material mat;
    mat.emission = emissionColor;
    mat.reflection = reflectionColor;
    mat.shinyness = shinyness;
    mat.roughness = roughness;
    mat.emissionPower = emissionPower;
    std::cout<<" matInit3 = " << mat.emission.r << " " << mat.emission.g << " " << mat.emission.b << " " << mat.emissionPower << std::endl;
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
