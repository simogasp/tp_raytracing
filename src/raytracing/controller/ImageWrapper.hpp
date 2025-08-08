
#include <cstdint>
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
        /**
         * The width of the image.
         */
        uint32_t width;
        /**
         * The height of the image. 
         */
        uint32_t height; 
        /**
         * The texture id.
         */
        GLuint textureId;
        /**
         * The data buffer.
         */
        uint32_t *imageData;

    public:

        /**
         * Creates a imageWrapper.
         */
        ImageWrapper();
        /**
         * Destroys a imageWrapper.
         */
        ~ImageWrapper() = default;
        /**
         * Store new data in the imageData pointer.
         */
        void setData(uint32_t *newData);
        /**
         * Gets the width of the image.
         * @return the width
         */
        unsigned int getWidth() const;
        /**
         * Gets the height of the image.
         * @return the height
         */
        unsigned int getHeight() const;
        /**
         * Gets the texture Id.
         * @return the texture id
         */
        GLuint getTextureId();
        
        // render procedure.
        /**
         * Handles a viewport resize action.
         */
        void resize(const uint32_t newWidth, const uint32_t newHeight); 

    };

}
