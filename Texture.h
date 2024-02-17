#pragma once
#include <string>
#include "Texture.h"
//#include "glad/glad.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Texture
{
private:
    unsigned int ID;
    std::string m_FilePath;
    unsigned char* localBuffer;
    int width, height, BPP;
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot=0);
    void Unbind() const;

    inline int GetWidth() const {   return width; }
    inline int GetHeight() const {   return height; }

    static bool SaveAsImage(std::string filename);
};
