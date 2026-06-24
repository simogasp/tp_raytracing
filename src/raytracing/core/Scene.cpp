# include "Scene.hpp"
# include <iostream>

const Raytracing::Scene::ObjectList& Raytracing::Scene::getListObjects() const
{
    // std::cout << "getListSphere()" << std::endl;
    return objectList;
}


Raytracing::Scene::ObjectList& Raytracing::Scene::getListObjects()
{
    return objectList;
}

const std::vector<Raytracing::Material>& Raytracing::Scene::getListMaterial() const
{
    return materialList;
}


std::vector<Raytracing::Material>& Raytracing::Scene::getListMaterial()
{
    return materialList;
}

void Raytracing::Scene::addObject(ObjectPtr object)
{
    objectList.push_back(std::move(object));
}

void Raytracing::Scene::addMaterial(const Raytracing::Material& material)
{
    materialList.push_back(material);
}
