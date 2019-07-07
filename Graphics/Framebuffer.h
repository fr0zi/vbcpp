#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED


#include <vector>

#include <GLEW/glew.h>

#include "Rect.h"

#include "../Utils/RTexture2D.h"


class Framebuffer
{
    friend class OGLDriver;

    private:
        GLuint _fboId;
        std::vector<RTexture*> _textures;
        GLuint _depthRenderbuffer;

        std::vector<GLenum> _fboBuffs;

        UintRect _viewport;

        bool _isInitialized;

        void checkFramebufferStatus();

    public:
        Framebuffer();
        ~Framebuffer();

        void init();
        void bind();

        void addDepthRenderbuffer(unsigned int width, unsigned int height, bool multisample = false, int samplesCount = 0);
        void addTexture(TextureFormat format, unsigned int width, unsigned int height, bool multisample = false, int samplesCount = 0);

        int getTexturesCount();
        RTexture* getTexture(int index = 0);

        void setViewport(const UintRect& viewport);
        UintRect& getViewport();

};


#endif // FRAMEBUFFER_H_INCLUDED
