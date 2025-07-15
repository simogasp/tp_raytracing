# pragma once

# include "glm/glm.hpp"
# include <vector>

namespace Raytracing
{
    class Camera
    {
    private:
        glm::vec3 position;
        glm::vec3 lookAt;
        glm::vec3 upVector;
        glm::vec3 x;
        glm::vec3 y;
        glm::vec3 z;
        uint32_t width;
        uint32_t height;
        float near;
        float far;
        float fieldOfView;
        float speed = 1;
        float rotationSpeed = 0.1;
        float cosRotationSpeed = cos(rotationSpeed);
        float sinRotationSpeed = sin(rotationSpeed);
        std::vector<glm::vec3> rayDirections;

    public:
        Camera(const glm::vec3& positionCamera, const glm::vec3& lookAtCamera, 
            const glm::vec3& upCamera, 
            const float fieldOfViewCamera, const float nearCamera, const float farCamera);
        Camera() = default;
        ~Camera();

        [[ nodiscard ]]
        glm::vec3 getPosition() const;
        
        [[ nodiscard ]]
        glm::vec3 getLookAt() const;

        [[ nodiscard ]]
        float getFieldOfView() const;

        [[ nodiscard ]]
        float getNear() const;

        [[ nodiscard ]]
        float getFar() const;

        [[ nodiscard ]]
        std::vector<glm::vec3> getRayDirections() const;

        

        // setters
        void setFov(const double newValue);
        void setUpVector(const glm::vec3& newUp);
        void setPosition(const glm::vec3& newPosition);
        void setLookAt(const glm::vec3& newLookAt);
        void setNear(const double newNear);
        void setFar(const double newFar);



        // mouvement of the camera
        void forward();
        void backward();
        void left();
        void right();
        void up();
        void down();
        // rotation of the camera
        void lookLeft();
        void lookRight();
        void rotateClockWise();
        void rotateAntiClockWise();
        
        // action
        void onResize(const uint32_t newWidth, const uint32_t newHeight);
        void updateRay();

    private:
        [[ nodiscard ]]
        glm::vec3 baseChangment(glm::vec3 vect);
        void computeBase();

    };
    

} // namespace Ratracing
