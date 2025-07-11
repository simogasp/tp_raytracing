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

void Raytracing::SceneFactory::pushSphere(const glm::vec3 center, const float radius, const glm::vec3 reflection, const glm::vec3 emission)
{
    currentScene.addSphere({center, radius, reflection, emission});
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
