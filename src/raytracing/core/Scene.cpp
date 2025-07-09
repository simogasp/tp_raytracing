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
    const float radius = 1;
    const Vector3 center = {0, 0, 0};
    const Sphere sphere = Sphere(radius, center, {255, 0, 255});
    sphereList.push_back(sphere);
    const Sphere sphere2 = Sphere(1, {0, 1, -0.6}, {255, 0, 255});
    sphereList.push_back(sphere2);

}
