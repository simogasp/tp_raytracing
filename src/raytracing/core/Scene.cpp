# include "Scene.hpp"

std::vector<Raytracing::Sphere> Raytracing::Scene::getListSphere() const
{
    return sphereList;
}

std::vector<Raytracing::Material> Raytracing::Scene::getListMaterial() const
{
    return materialList;
}

void Raytracing::Scene::addSphere(const Raytracing::Sphere& sphere)
{
    sphereList.push_back(sphere);
}

void Raytracing::Scene::addMaterial(const Raytracing::Material& material)
{
    materialList.push_back(material);
}

void Raytracing::Scene::addRandomSphere()
{
    Sphere sphere;
    sphere.radius = (float) rand() / RAND_MAX * 2.8f + 0.2f;
    sphere.center = glm::vec3(rand() / RAND_MAX * 10.f, rand() / RAND_MAX * 10.f, rand() / RAND_MAX * 10.f);
    sphere.materialIndex = rand() / RAND_MAX * materialList.size();
    // create a green material if no one exist.
    if (materialList.size() == 0)
    {
        Material mat;
        mat.emission = glm::vec3(0.f);
        mat.reflection = glm::vec3(0, 255, 0);
        mat.shinyness = 0;
        materialList.push_back(mat);
    }
    sphereList.push_back(sphere);
}
