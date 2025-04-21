/**
 * Author:	Varun Khambhata
 * Created: 19.04.2025
**/

#include <GL/glew.h>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <Ignition0Core/stb_image.h>
#include <Ignition0Core/Logger0.h>
#include <Ignition0Supplement/ImageUtil0.h>

GLuint ImageUtil0::LoadImageTexture(const char* imgFile) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imgFile, &width, &height, &nrChannels, 0);	

    GLuint texture;
    if(data) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else {
        Logger0E("Missing Texture File: " << imgFile);
    }
    return texture;
}