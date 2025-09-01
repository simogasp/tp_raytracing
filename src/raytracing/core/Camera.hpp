#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <cmath>

namespace Raytracing
{
    class Camera
    {
    private:
        /**
         * The camera Position.
         */
        glm::vec3 position{};
        
        /**
         * The lookAt vector.
         */
        glm::vec3 lookAt{.0f, .0f, -1.f};
        
        /**
         * the up vector.
         */
        glm::vec3 upVector{.0f, 1.f, .0f};
        
        /**
         * the x axis of the camera.
         */
        glm::vec3 x{1.f, .0f, .0f};
        
        /**
         * the y axis of the camera.
         */
        glm::vec3 y{.0f, 1.f, .0f};
        
        /**
         * The z axis of the camera.
         */
        glm::vec3 z{.0f, .0f, 1.f};
        
        /**
         * the width of the image.
         */
        uint32_t width;
        
        /**
         * The height of the image.
         */
        uint32_t height;
        
        /**
         * the near value.
         */
        float near;
        
        /**
         * The far value.
         */
        float far;

        /**
         * The camera focal.
         */
        float focal;
        /**
         * The movement speed.
         */
        float speed{1};
        /**
         * The rotation speed.
         */
        float rotationSpeed{.1f};
        
        /**
         * The cosine of the rotation speed.
         */
        float cosRotationSpeed{std::cos(rotationSpeed)};
        /**
         * The sine of the rotation speed.
         */
        float sinRotationSpeed{std::sin(rotationSpeed)};
        /**
         * The ray directions.
         */
        std::vector<glm::vec3> rayDirections;

    public:
        /**
         * Creates a camera with the given properties.
         */
        Camera(const glm::vec3& positionCamera, const glm::vec3& lookAtCamera, 
            const glm::vec3& upCamera, 
            float fieldOfViewCamera, float nearCamera, float farCamera);
        /**
         * Creates a Camera.
         */
        Camera() = default;
        /**
         * Destroys a Camera.
         */
        ~Camera() = default;

        /**
         * Get the position of the camera.
         * @return a vector
         */
        [[ nodiscard ]]
        glm::vec3 getPosition() const;
        
        /**
         * Get the lookAt vector.
         * @return a vector
         */
        [[ nodiscard ]]
        glm::vec3 getLookAt() const;

        /**
         * Gets the field of view of the camera.
         * @return the field of view
         */
        [[ nodiscard ]]
        float getFocal() const;

        /**
         * Gets the near value.
         * @return the near value
         */
        [[ nodiscard ]]
        float getNear() const;

        /**
         * Gets the far value.
         * @return the far value
         */
        [[ nodiscard ]]
        float getFar() const;

        /**
         * get the ray directions from the camera.
         * @return a vector of vector3
         */
        [[ nodiscard ]]
        std::vector<glm::vec3> getRayDirections() const;

        

        // setters
        /**
         * Sets the horizontal fov.
         * @param newValue the new fov
         */
        void setFocal(float newValue);
        
        /**
         * Sets the up vector to the given one.
         * @param newUp the new up vector
         */
        void setUpVector(const glm::vec3& newUp);
        
        /**
         * Sets the position of the camera.
         * @param newPosition the new position of the camera
         */
        void setPosition(const glm::vec3& newPosition);
        
        /**
         * Sets the lookAt vector of the camera.
         * @param newLookAt the new lookAt vector.
         */
        void setLookAt(const glm::vec3& newLookAt);

        
        /**
         * Sets the near value.
         * @param newNear the new value
         */
        void setNear(double newNear);
        
        /**
         * Sets the far value.
         * @param newFar the new value
         */
        void setFar(double newFar);



        // movement of the camera
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
        
        // rotation of the camera
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
         * Rotates the camera clock wise.
         */
        void rotateClockWise();
        
        /**
         * Rotates the camera anti clock wise.
         */
        void rotateAntiClockWise();
        
        // render action
        /**
         * Handles a resize action.
         * @param newWidth the new width
         * @param newHeight the new height
         */
        void onResize(uint32_t newWidth, uint32_t newHeight);
        /**
         * Compute the ray directions.
         */
        void updateRay();

    private:
        /**
         * Change a vector to camera base.
         * @param vect the vector to change
         */
        [[ nodiscard ]]
        glm::vec3 baseChangment(glm::vec3 vect) const;
        /**
         * Computes the bases from the LookAt and up vector.
         */
        void computeBase();

    };
    

} // namespace Ratracing
