#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED


#include <string.h>
#include <list>

#include "OGLDriver.h"
#include "RenderData.h"
#include "RShader.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/RefCounter.h"

#include "../GUI/GUIObject.h"
#include "../GUI/Image.h"


class Renderer : virtual public RefCounter
{
    private:
        //OGLDriver* _OGLDriver;
        std::vector<RShader*> _shaderList;
        UBO* _lightUBO;

        unsigned int _screenWidth;
        unsigned int _screenHeight;

    public:
        Renderer(unsigned int screenWidth, unsigned int screenHeight/* OGLDriver* driver */);
        ~Renderer();

        void render(RenderData* renderData);
        void renderGUI(std::list<GUIObject*>* GUIObjectsList);

};


#endif // RENDERER_H_INCLUDED
