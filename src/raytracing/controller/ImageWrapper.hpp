
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
        void setData(uint32_t *newData);
        uint getWidth() const;
        uint getHeight() const;
        GLuint getTextureId();
        void resize(const uint32_t newWidth, const uint32_t newHeight); 

    };

}
