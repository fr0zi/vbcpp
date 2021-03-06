#include "RTexture2D.h"

#include <iostream>


RTexture2D::RTexture2D(string path, unsigned char* data, TextureFormat format, glm::uvec2 size)
    : RTexture(path, TT_2D, format, size)
{
    std::cout << "RTexture2D - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    glTexImage2D(_type, 0, _format, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, data);
}


RTexture2D::RTexture2D(TextureFormat format, glm::uvec2 size)
    : RTexture("", TT_2D, format, size)
{
    std::cout << "RTexture2D - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    glTexImage2D(_type, 0, _format, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, NULL);
}


RTexture2D::~RTexture2D()
{

}


void RTexture2D::setTexSubImage(unsigned char* data, int offsetX, int offsetY, int width, int height)
{
    if (offsetX + width > _size.x || offsetY + height > _size.y)
        return;

    glTexSubImage2D(_type, 0, offsetX, offsetY, width, height, _format, GL_UNSIGNED_BYTE, data);
}
