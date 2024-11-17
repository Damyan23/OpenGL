#pragma once

#ifndef TEXTUREGEN_H
#define TEXTUREGEN_H

#include <glad/glad.h> // Helps load the correct OpenGL function pointers
#include <iostream>
#include "stb_image.h"

class TextureGenerator
{
private:
    int width, height, nrChannels;
    unsigned int texture;

public:
    // Constructor that generates and loads the texture
    TextureGenerator(const char* imagePath);
    ~TextureGenerator();


    void use();
    int ID() const;
};

#endif
