/**
 * This is the App We wants to build with a raytracing.
 */

#include "raytracing/controller/Renderer.hpp"

namespace Raytracing
{

    class App
    {
    private:
        /* data */
        uint32_t m_viewportWidth = 0;
        uint32_t m_viewportHeight = 0;

        Renderer renderer;
        int fovDegree = 179;
        float fov = toRadian(fovDegree);

    public:
        App() = default;

        ~App() = default;

        void OnUIRender();
        void Render();
        

        void keyboardHandler();
        
    };
}
