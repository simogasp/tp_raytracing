#pragma once

#include "raytracing/core/Camera.hpp"
#include "glm/glm.hpp"

namespace Raytracing
{
    class CameraFactory
    {
    private:
        /**
         * The camera created.
         */
        Camera currentCamera;

    public:
        /**
         * Creates a Camera factory.
         */
        CameraFactory() = default;
        
        /**
         * Destroys a Camera factory.
         */
        ~CameraFactory() = default;

        /**
         * Returns a camera with starting value.
         */
        static Raytracing::Camera getDefaultCamera();

        // properties
        /**
         * Creates a new Camera.
         */
        void createNewCamera();
        
        /**
         * Sets the position of the current camera to the given one.
         * @param pos the new pos
         */
        void setCameraPosition(const glm::vec3& pos);
        
        /**
         * Sets the look at vector of the camera.
         */
        void setLookAt(const glm::vec3& lookAt);
        
        /**
         * Sets the horizontal FoV by a radians value.
         * @param fov the new fov in radians
         */
        void setRadiansHorizontalFOV(const double fov);
        
        /**
         * Sets the horizontal FoV by a degree value.
         * @param fov the degree value.
         */
        void setDegreeHorizontalFOV(const double fov);
        
        /**
         * Sets the near value of the camera.
         * @param near the near value
         */
        void setNear(const double near);
        
        /**
         * Sets the far value of the camera.
         * @param far the far value
         */
        void setFar(const double far);
        
        /**
         * Sets the up vector of the camera.
         * @param up the new upVector
         */
        void setUpVector(const glm::vec3 up);

        // movement
        /**
         * Moves the camera forward.
         */
        void forward();
        
        /**
         * Moves the camera backward.
         */
        void backward();
        
        /**
         * Moves the camera to the left.
         */
        void left();
        
        /**
         * Moves the camera to the right.
        */
        void right();
        
        /**
         * Moves the camera upward.
         */
        void up();
        
        /**
         * Moves the camera downward.
         */
        void down();
        
        /**
         * Rotate to look up.
         */
        void lookUp();

        /**
         * Rotate to look down.
         */
        void lookDown();
        
        /**
         * Rotates the camera to the left.
         */
        void lookLeft();
        
        /**
         * Rotates the camera to the right.
         */
        void lookRight();
        
        /**
         * Rotates camera anti clock wise.
         */
        void rotateAntiClockWise();
        
        /**
         * Rotates camera clock wise.
         */
        void rotateClockWise();
        
        // render procedures
        /**
         * Computes the ray directions.
         */
        void updateRay();
        /**
         * Handle a resize of the viewport.
         */
        void onResize(const uint32_t width, const uint32_t height);
        
        /**
         * Returns the camera made.
         * @return the camera
         */
        Raytracing::Camera getCamera();
    };
} // namespace Raytracing
