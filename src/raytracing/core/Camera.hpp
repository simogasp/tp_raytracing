# pragma once

# include "raytracing/utils/vector.hpp"

namespace Raytracing
{
    class Camera
    {
    private:
        Vector3 position;
        Vector3 lookAt;
        Vector3 upVector;
        Vector3 x;
        Vector3 y;
        Vector3 z;
        float focal;
        float near;
        float far;
        float fieldOfView;
        float speed = 1;
        float rotationSpeed = 0.1;
        float cosRotationSpeed = cos(rotationSpeed);
        float sinRotationSpeed = sin(rotationSpeed);

    public:
        Camera(const Vector3& positionCamera, const Vector3& lookAtCamera, 
            const Vector3& upCamera, const float focalCamera, 
            const float fieldOfViewCamera, const float nearCamera, const float farCamera);
        ~Camera();

        [[ nodiscard ]]
        Vector3 getPosition();
        
        [[ nodiscard ]]
        Vector3 getLookAt();

        [[ nodiscard ]]
        float getFocal();

        [[ nodiscard ]]
        float getFieldOfView();

        [[ nodiscard ]]
        float getNear();

        [[ nodiscard ]]
        float getFar();

        [[ nodiscard ]]
        Vector3 baseChangment(Vector3 vect);


        void setFov(const double newValue);

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

    private:
        void computeBase();

    };
    

} // namespace Ratracing
