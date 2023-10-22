/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/UnlitColor.h>
#include <Ignition0Supplement/VoidMemory0.h>

static VoidMemory0 ucSharedMem;
UnlitColor*  currentUsedMaterial;

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
	uniforms.r = r;
	uniforms.g = g;
	uniforms.b = b;

	if(!ucSharedMem++) {
		setShaderProgram(static_cast<unsigned int*>(ucSharedMem.getMemory())[0]);
		sharedUniforms.projection(static_cast<unsigned int*>(ucSharedMem.getMemory())[1]);
		uniR = static_cast<unsigned int*>(ucSharedMem.getMemory())[2];
		uniG = static_cast<unsigned int*>(ucSharedMem.getMemory())[3];
		uniB = static_cast<unsigned int*>(ucSharedMem.getMemory())[4];
		return;
	}

	build();
	ucSharedMem.setMemory(new unsigned int[5] {getShaderProgram(), (unsigned int)getLocation("projection"), 
							(unsigned int)getLocation("r"), (unsigned int)getLocation("g"), (unsigned int)getLocation("b")});
}

UnlitColor::~UnlitColor() {
	if(!ucSharedMem--) {
		destroy();
    }
}

void UnlitColor::onUsed() {
	if(currentUsedMaterial == this) return;

	currentUsedMaterial = this;
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
