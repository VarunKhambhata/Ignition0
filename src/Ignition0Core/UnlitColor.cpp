/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>
#include <Ignition0Core/UnlitColor.h>

static UnlitColor* currentUsedMaterial;
static ShaderContext singleUCShaderContext;

std::string UnlitColor::vertexShaderSource() {
	return R"(
		#version 430 core
	    in vec3 aVertex;
	    uniform mat4 model;

		mat4 getProjection();
		mat4 getView();
		mat4 getInstancedTransfrom();

	    void main()
	    {
			gl_Position = getProjection() * getView() * getInstancedTransfrom() * model * vec4(aVertex, 1.0);
	    }
	)";
}

std::string UnlitColor::fragmentShaderSource() {
	return R"(
		#version 430 core
		out vec4 FragColor;
	    uniform float r = 0, g = 0, b = 0;

	    void main()
	    {
			FragColor = vec4(r,g,b,1.0);
	    }
    )";
}

AttribNames UnlitColor::bindShaderAttribs() {
	return {"aVertex"};
}

UnlitColor::UnlitColor(float r, float g, float b) {
	build(&singleUCShaderContext);

	red = r;
	green = g;
	blue = b;
}

void UnlitColor::onUsed() {
	if(currentUsedMaterial == this) return;
	currentUsedMaterial = this;

	glUniform1f(red, ~red);
	glUniform1f(green, ~green);
	glUniform1f(blue, ~blue);
}

void UnlitColor::initUniforms() {
	sharedUniforms.model(getLocation("model"));

	red(getLocation("r"));
	green(getLocation("g"));
	blue(getLocation("b"));
}