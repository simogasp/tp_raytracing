# include "Scene.hpp"

std::vector<Raytracing::Sphere> Raytracing::Scene::getListSphere()
{
    return sphereList;
}

std::vector<Raytracing::Triangle> Raytracing::Scene::getListTriangle()
{
    return triangleList;
}

void Raytracing::Scene::addSphere(Raytracing::Sphere sphere)
{
    sphereList.push_back(sphere);
}

void Raytracing::Scene::addRandomSphere()
{
    Sphere sphere;
    sphere.radius = (float) rand() / RAND_MAX * 1.3f + 0.2f;
    sphere.center = glm::vec3(0.f, 0.f, 0.f);
    sphere.reflection = {255, 0, 255};
    sphereList.push_back(sphere);
}
