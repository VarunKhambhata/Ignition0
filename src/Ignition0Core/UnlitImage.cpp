/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/UnlitImage.h>

std::string UnlitImage::vertexShaderSource() {
	return R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec2 aTexCoords;
		uniform mat4 projection;
		out vec2 TexCoords;

		void main()
		{
		    gl_Position = projection * vec4(aPos.x, aPos.y, 0.0, 1.0);
		    TexCoords = aTexCoords;
		}
	)";
}

std::string UnlitImage::fragmentShaderSource() {
	return R"(
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;
		uniform sampler2D screenTexture;

		void main()
		{ 
		    FragColor = texture(screenTexture, TexCoords);
		}
    )";
}

UnlitImage::UnlitImage(bool init) {
	init? build() : void();
}

void UnlitImage::onUsed() {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void UnlitImage::setTexture(GLuint tex) {
	texture = tex;
}

void UnlitImage::initUniforms() {
	sharedUniforms.projection(getLocation("projection"));
}