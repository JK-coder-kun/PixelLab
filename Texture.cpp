#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
Texture::Texture()
:ID(0), localBuffer(nullptr), width(0), height(0), BPP(0)
{

}

Texture::~Texture()
{
    glDeleteTextures(1,&ID);
}

void Texture::LoadTexture(const std::string& path)
{
    stbi_set_flip_vertically_on_load(1); //openGL start from the bottom left and png format start from the top left that is why

    localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);    //To load an image as a 4-channel RGBA image
    //int format = BPP == 24 ? GL_BGR : BPP == 8 ? GL_LUMINANCE : 0;
	//int internalFormat = BPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);

    if (localBuffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(localBuffer);
}

bool Texture::SaveAsImage(std::string filename)
{
    int width, height;
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &height);
    unsigned char* pixels = (unsigned char*)malloc(width * height * 4);;
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_flip_vertically_on_write(1);
    stbi_write_png(  filename.c_str(), width, height, 4, pixels, 0);
    free(pixels);
        /*int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        unsigned char* ptr = (unsigned char*)malloc(width * height * 4);
        glReadPixels(0, 0, width, height, GL_RGBA,  GL_UNSIGNED_BYTE, ptr);
        stbi_write_png(  "Textures/screenshot.png", width, height, 4, ptr, 0);
        free(ptr);*/


        return true;
}

void Texture::Bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);//select the slot for texture
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
