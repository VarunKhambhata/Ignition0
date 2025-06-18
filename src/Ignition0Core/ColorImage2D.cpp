/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <Ignition0Core/ColorImage2D.h>

static ShaderContext singleFPShaderContext;

std::string ColorImage2D::vertexShaderSource() {
	return R"(
		#version 430 core
		in vec3 aVertex;
		in vec2 aTexCoords;
		uniform mat4 mvp;
		out vec2 TexCoords;

		void main()
		{
			gl_Position = vec4(aVertex,1.0);
		    TexCoords = aTexCoords;
		}
	)";
}

std::string ColorImage2D::fragmentShaderSource() {
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

AttribNames ColorImage2D::bindShaderAttribs() {
	return {"aVertex", "aTexCoords"};
}

ColorImage2D::ColorImage2D() {
	build(&singleFPShaderContext);
}

void ColorImage2D::onUsed() {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void ColorImage2D::setTexture(GLuint tex) {
	texture = tex;
}