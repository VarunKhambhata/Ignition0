/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/UnlitImage.h>
#include <Ignition0Core/stb_image.h>
#include <Ignition0Core/Logger0.h>

#include <Ignition0Supplement/VoidMemory0.h>

static VoidMemory0 uiSharedMem;

std::string UnlitImage::vertexShaderSource() {
	return R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoords;
		uniform mat4 projection;
		out vec2 TexCoords;

		void main()
		{
		    gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
		    TexCoords = aTexCoords;
		}
	)";
}

std::string UnlitImage::fragmentShaderSource() {
	return R"(
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;
		uniform sampler2D tex;

		void main()
		{ 
		    FragColor = texture2D(tex, TexCoords);
		}
    )";
}

UnlitImage::UnlitImage(bool init) {
	if(!uiSharedMem++) {
		setShaderProgram(static_cast<unsigned int*>(uiSharedMem.getMemory())[0]);
		sharedUniforms.projection(static_cast<unsigned int*>(uiSharedMem.getMemory())[1]);
		return;
	}

	init? build() : void();
	uiSharedMem.setMemory(new unsigned int[2] {getShaderProgram(), (unsigned int)getLocation("projection")});
}

UnlitImage::~UnlitImage() {
	if(!uiSharedMem--) {
		destroy();
    }
}

void UnlitImage::onUsed() {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void UnlitImage::setTexture(GLuint tex) {
	texture = tex;
}

void UnlitImage::setTexture(const char* imgFile) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imgFile, &width, &height, &nrChannels, 0);	

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
}

void UnlitImage::initUniforms() {
	sharedUniforms.projection(getLocation("projection"));
}