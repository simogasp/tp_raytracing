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
    const float radius = (float) rand() / RAND_MAX * 10;
    const Vector3 center = {0, 0, 0};
    const Sphere sphere = Sphere(radius, center);
    sphereList.push_back(sphere);
}
