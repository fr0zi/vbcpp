#include "RTextureCubeMap.h"

#include <iostream>


RTextureCubeMap::RTextureCubeMap(std::string path, unsigned char** data, TextureFormat format, unsigned int size)
    : RTexture(path, TT_CUBE, format, glm::uvec2(size, size))
{
    std::cout << "RTextureCubeMap - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(CMF_POSITIVE_X + i, 0, _format, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, data[i]);
    }
}


RTextureCubeMap::RTextureCubeMap(TextureFormat format, unsigned int size)
    : RTexture("", TT_CUBE, format, glm::uvec2(size, size))
{
    std::cout << "RTextureCubeMap - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(CMF_POSITIVE_X + i, 0, _format, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, NULL);
    }

}


RTextureCubeMap::~RTextureCubeMap()
{

}


void RTextureCubeMap::setTexSubImage(unsigned char* data, CubeMapFace face, int offsetX, int offsetY, int width, int height)
{
    if (offsetX + width > _size.x || offsetY + height > _size.y)
        return;

    glTexSubImage2D(face, 0, offsetX, offsetY, width, height, _format, GL_UNSIGNED_BYTE, data);
}
