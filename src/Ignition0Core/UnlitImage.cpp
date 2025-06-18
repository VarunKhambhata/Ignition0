/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Supplement/ImageUtil0.h>
#include <Ignition0Core/UnlitImage.h>

static ShaderContext singleUIShaderContext;

std::string UnlitImage::vertexShaderSource() {
	return R"(
		#version 430 core
		in vec3 aVertex;
		in vec2 aTexCoords;

		out vec2 TexCoords;

		uniform mat4 model;

		mat4 getProjection();
		mat4 getView();
		mat4 getInstancedTransfrom();

		void main()
		{
			gl_Position = getProjection() * getView() * getInstancedTransfrom() * model * vec4(aVertex, 1.0);
		    TexCoords = aTexCoords;
		}
	)";
}

std::string UnlitImage::fragmentShaderSource() {
	return R"(
		#version 430 core
		in vec2 TexCoords;
		out vec4 FragColor;
		uniform sampler2D tex;

		void main()
		{ 
		    FragColor = texture(tex, TexCoords);
		}
    )";
}

AttribNames UnlitImage::bindShaderAttribs() {
	return {"aVertex", "aTexCoords"};
}

UnlitImage::UnlitImage() {
	build(&singleUIShaderContext);
}

UnlitImage::~UnlitImage() {
	glDeleteTextures(1, &texture);
}

void UnlitImage::onUsed() {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void UnlitImage::loadImage(const char* imgFile) {
	texture = ImageUtil0::LoadImageTexture(imgFile);
}

void UnlitImage::initUniforms() {
	sharedUniforms.model(getLocation("model"));
}