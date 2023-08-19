/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/UnlitColor.h>

std::string UnlitColor::vertexShaderSource() {
	return R"(
		#version 330 core
	    layout (location = 0) in vec3 aPos;
	    uniform mat4 projection;
	    void main()
	    {
	       gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	    }
	)";
}

std::string UnlitColor::fragmentShaderSource() {
	return R"(
		#version 330 core
	    out vec4 FragColor;
	    uniform float r = 0, g = 0, b = 0;
	    void main()
	    {
			gl_FragColor = vec4(r,g,b,1.0);
	    }
    )";
}

UnlitColor::UnlitColor(float r, float g, float b) {
	build();
	uniforms.r = r;
	uniforms.g = g;
	uniforms.b = b;
	updateUniforms();
}

void UnlitColor::initUniforms() {
	sharedUniforms.projection(getLocation("projection"));
	uniR = getLocation("r");
	uniG = getLocation("g");
	uniB = getLocation("b");
}

void UnlitColor::onUniformsUpdate() {
	glUniform1f(uniR, uniforms.r);
	glUniform1f(uniG, uniforms.g);
	glUniform1f(uniB, uniforms.b);
}