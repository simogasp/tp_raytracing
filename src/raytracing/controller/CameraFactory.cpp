# include "CameraFactory.hpp"
# include "raytracing/core/Ray.hpp"
# include <iostream>


Raytracing::Camera Raytracing::CameraFactory::getDefaultCamera()
{
    return Camera({0, 0, 2}, {0, 0, 0}, {0, 1, 0}, 45, 2.f, 1000);
}

void Raytracing::CameraFactory::createNewCamera()
{
    currentCamera = Camera();
}

void Raytracing::CameraFactory::setCameraPosition(const glm::vec3& pos)
{
    currentCamera.setPosition(pos);
}

void Raytracing::CameraFactory::setLookAt(const glm::vec3& lookAt)
{
    currentCamera.setLookAt(lookAt);
}

void Raytracing::CameraFactory::setNear(const double near)
{
    currentCamera.setNear(near);
}

void Raytracing::CameraFactory::setFar(const double far)
{
    currentCamera.setFar(far);
}

void Raytracing::CameraFactory::setCameraFocal(const float newFocal) 
{
    currentCamera.setFocal(newFocal);
}

void Raytracing::CameraFactory::setUpVector(const glm::vec3 up)
{
    currentCamera.setUpVector(up);
}

void Raytracing::CameraFactory::forward()
{
    currentCamera.forward();
}

void Raytracing::CameraFactory::backward()
{
    currentCamera.backward();
}

void Raytracing::CameraFactory::left()
{
    currentCamera.left();
}

void Raytracing::CameraFactory::right()
{
    currentCamera.right();
}

void Raytracing::CameraFactory::up()
{
    currentCamera.up();
}

void Raytracing::CameraFactory::down()
{
    currentCamera.down();
}

void Raytracing::CameraFactory::lookUp()
{
    currentCamera.lookUp();
}

void Raytracing::CameraFactory::lookDown()
{
    currentCamera.lookDown();
}

void Raytracing::CameraFactory::lookLeft()
{
    currentCamera.lookLeft();
}

void Raytracing::CameraFactory::lookRight()
{
    currentCamera.lookRight();
}

void Raytracing::CameraFactory::rotateAntiClockWise()
{
    currentCamera.rotateAntiClockWise();
}

void Raytracing::CameraFactory::rotateClockWise()
{
    currentCamera.rotateClockWise();
}

Raytracing::Camera Raytracing::CameraFactory::getCamera() const
{
    return currentCamera;
}

void Raytracing::CameraFactory::updateRay()
{
    currentCamera.updateRay();
}

void Raytracing::CameraFactory::onResize(const uint32_t width, const uint32_t height)
{
    currentCamera.onResize(width, height);

}
