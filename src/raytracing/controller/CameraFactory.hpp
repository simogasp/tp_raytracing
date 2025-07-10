#pragma once

#include "raytracing/core/Camera.hpp"
#include "glm/glm.hpp"

namespace Raytracing
{
    class CameraFactory
    {
    private:
        Camera currentCamera;

    public:
        CameraFactory() = default;
        ~CameraFactory() = default;

        static Raytracing::Camera getDefaultCamera();

        // properties
        void createNewCamera();
        void setCameraPosition(const glm::vec3& pos);
        void setLookAt(const glm::vec3& lookAt);
        void setRadiansHorizontalFOV(const double fov);
        void setDegreeHorizontalFOV(const double fov);
        void setNear(const double near);
        void setFar(const double far);
        void setUpVector(const glm::vec3 up);

        // movement
        void forward();
        void backward();
        void left();
        void right();
        void up();
        void down();
        void lookLeft();
        void lookRight();
        void rotateAntiClockWise();
        void rotateClockWise();
        void updateRay();
        void onResize(const uint32_t width, const uint32_t height);
        Raytracing::Camera getCamera();
    };
} // namespace Raytracing
