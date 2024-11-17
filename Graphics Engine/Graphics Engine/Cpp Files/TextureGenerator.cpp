#include "C:\Users\dpeyc\OneDrive\Desktop\Graphics Engine\Graphics Engine\Header Files\TextureGenerator.h"

#include <iostream>
#include <C:\Users\dpeyc\OneDrive\Desktop\Graphics Engine\Graphics Engine\Header Files\stb_image.h>

TextureGenerator::TextureGenerator(const char* imagePath)
{
    // Create the texture
    glGenTextures(1, &texture);
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the image
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        // Create texture from image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Free the image memory
    stbi_image_free(data);
};

TextureGenerator::~TextureGenerator()
{

};

// Get the texture ID
void TextureGenerator::use ()
{
    glBindTexture(GL_TEXTURE_2D, texture);
};

int TextureGenerator::ID() const
{
    return texture;
}