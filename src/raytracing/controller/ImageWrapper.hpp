
# include <stdint.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

namespace Raytracing
{

    class ImageWrapper
    {
    private:
        uint32_t width;
        uint32_t height; 
        GLuint textureId;
        uint32_t *imageData;

    public:
        ImageWrapper();
        ~ImageWrapper() = default;
        void setPixel(const int i, const uint32_t color);
        void update();
        uint getWidth();
        uint getHeight();
        GLuint getTextureId();
        void resize(const uint32_t newWidth, const uint32_t newHeight); 

    };

}
