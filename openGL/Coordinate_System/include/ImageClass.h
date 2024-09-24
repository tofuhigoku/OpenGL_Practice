#ifndef IMAGECLASS_H
#define IMAGECLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "stb_image.h"

using namespace std;

class ImageClass
{
private:
    /* data */
public:
    ImageClass(const char* image_path, bool flip);
    ~ImageClass();

    int width, height, nrChannels;
    unsigned char *data;
    unsigned int texture;

};

ImageClass::ImageClass(const char* image_path, bool flip)
{
    if(flip != true && flip != false)
    {
        cout << "invalid parameter: flip" << endl;
    }
    else
    {
        stbi_set_flip_vertically_on_load(flip);
    }
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(image_path, &width, &height, &nrChannels, 0);
    if (data)
    {
        cout << "nrChannels = " << nrChannels << endl;
        if(nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if( nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "NUm of Channel not equal 3 or 4" << endl;
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

}

ImageClass::~ImageClass()
{
}

// int width, height, nrChannels;
// unsigned char *data = stbi_load("container.jpg", &width, &height,
// &nrChannels, 0);


#endif