# pragma once

# include "raytracing/utils/vector.hpp"

namespace Raytracing
{
    class Camera
    {
    private:
        Vector3 position;
        Vector3 lookAt;
        Vector3 up;
        float focal;
        float near;
        float far;
        float fieldOfView;
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


    };
    

} // namespace Ratracing
