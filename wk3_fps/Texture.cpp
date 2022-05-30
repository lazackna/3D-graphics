#include "Texture.h"
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string & fileName)
{
	//TODO
    name = fileName;
    stbi_uc* imgData;
    int width, height, bpp;
    imgData = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);
	glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
	//think about saving filename and only opening the file when binding
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        imgData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(imgData);
}

void Texture::bind()
{
    //glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, id);

    //imgData = stbi_load("Atlas.png", &width, &height, &bpp, 4);

    
}
