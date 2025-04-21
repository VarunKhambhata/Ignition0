/**
 * Author:	Varun Khambhata
 * Created: 25.10.2023
**/

#include <GL/glew.h>

#include <Ignition0Core/LitColor.h>

static LitColor* currentUsedMaterial;
static ShaderContext singleLCShaderContext;

std::string LitColor::vertexShaderSource() {
	return R"(
		#version 430 core
	    in vec3 aVertex;
	    in vec3 aNormal;

	    out vec3 FragPos;
	    out vec3 Normal;
		out vec3 ViewPos;

	    uniform mat4 model;

		mat4 getProjection();
		mat4 getView();
		vec3 getCamPosition();
		mat4 getInstancedTransfrom();

	    void main()
	    {
			mat4 instModel = getInstancedTransfrom() * model;
			gl_Position = getProjection() * getView() * instModel * vec4(aVertex, 1.0);
	    	FragPos = vec3(instModel * vec4(aVertex, 1.0));
	    	Normal = mat3(transpose(inverse(instModel))) * aNormal;
			ViewPos = getCamPosition();
	    }
	)";
}

std::string LitColor::fragmentShaderSource() {
	return R"(
		#version 430 core

	    in vec3 FragPos;
	    in vec3 Normal;
	    in vec3 ViewPos;

		out vec4 FragColor;

	    uniform vec3  color;
		uniform float ambientStrength;
		uniform float diffStrength;
		uniform float specularStrength;
		uniform float shininess;

		int getMaxDirectionalLights();
		int getMaxPointLights();

		vec3 DirectionalLightEffect(int index, vec3 viewDir, vec3 norm, float ambientStrength, float diffStrength, float specularStrength, float shininess);
		vec3 PointLightEffect(int pointLightIndex, vec3 viewDir, vec3 norm, vec3 FragPos, float ambientStrength, float diffStrength, float specularStrength, float shininess);

	    void main()
	    {
	    	vec3 effect;
	    	vec3 norm    = normalize(Normal);
	    	vec3 viewDir = normalize(ViewPos - FragPos);
			
	    	for(int i = 0; i < getMaxDirectionalLights(); i++)
				effect += DirectionalLightEffect(i, viewDir, norm, ambientStrength, diffStrength, specularStrength, shininess);
	    	
	    	for(int i = 0; i < getMaxPointLights(); i++)
				effect += PointLightEffect(i, viewDir, norm, FragPos, ambientStrength, diffStrength, specularStrength, shininess);

		    FragColor = vec4(effect * color, 1.0);
	    }
    )";
}

AttribNames LitColor::bindShaderAttribs() {
	return {"aVertex", "", "aNormal"};
}

LitColor::LitColor() {
	build(&singleLCShaderContext);	
}

void LitColor::setMaterialProperties(float r, float g, float b, float ambient, float diffuse, float specular, float shininess) {
	this->color     = glm::vec3(r, g, b);
	this->ambient   = ambient;
	this->diffuse   = diffuse;
	this->specular  = specular;
	this->shininess = shininess;
}

void LitColor::onUsed() {
	if(currentUsedMaterial == this) return;
	currentUsedMaterial = this;

	glUniform3fv(color, 1, & (~color)[0]);
	glUniform1f(ambient, ~ambient);
	glUniform1f(diffuse, ~diffuse);
	glUniform1f(specular, ~specular);
	glUniform1f(shininess, ~shininess);
}

void LitColor::initUniforms() {
	sharedUniforms.model(getLocation("model"));

	color(getLocation("color"));
	ambient(getLocation("ambientStrength"));
	diffuse(getLocation("diffStrength"));
	specular(getLocation("specularStrength"));
	shininess(getLocation("shininess"));
}