/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Supplement/ImageUtil0.h>
#include <Ignition0Core/LitImage.h>

static LitImage* currentUsedMaterial;
static ShaderContext singleLIShaderContext;

std::string LitImage::vertexShaderSource() {
	return R"(
		#version 430 core
	    in vec3 aVertex;
	    in vec3 aNormal;
		in vec2 aTexCoords;
		in mat4 T;

	    out vec3 FragPos;
	    out vec3 Normal;
		out vec3 ViewPos;
		out vec2 TexCoords;

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
			TexCoords = aTexCoords;
		}
	)";
}

std::string LitImage::fragmentShaderSource() {
	return R"(
		#version 430 core

		in vec3 FragPos;
	    in vec3 Normal;
		in vec3 ViewPos;
		in vec2 TexCoords;

		out vec4 FragColor;

		uniform vec3 viewPos;
		uniform sampler2D tex;

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
		    vec3 effect = vec3(0);
	    	vec3 norm    = normalize(Normal);
	    	vec3 viewDir = normalize(viewPos - FragPos);

	    	for(int i = 0; i < getMaxDirectionalLights(); i++)
				effect += DirectionalLightEffect(i, viewDir, norm, ambientStrength, diffStrength, specularStrength, shininess);
	    	
	    	for(int i = 0; i < getMaxPointLights(); i++)
				effect += PointLightEffect(i, viewDir, norm, FragPos, ambientStrength, diffStrength, specularStrength, shininess);

		    FragColor = texture(tex, TexCoords);
		    FragColor.rgb *= effect;
		}
    )";
}

AttribNames LitImage::bindShaderAttribs() {
	return {"aVertex", "aTexCoords", "aNormal"};
}

LitImage::LitImage() {
	build(&singleLIShaderContext);
}

void LitImage::setMaterialProperties(float ambient, float diffuse, float specular, float shininess) {
	this->ambient   = ambient;
	this->diffuse   = diffuse;
	this->specular  = specular;
	this->shininess = shininess;
}

void LitImage::onUsed() {
	glBindTexture(GL_TEXTURE_2D, texture);

	if(currentUsedMaterial == this) return;
	currentUsedMaterial = this;

	glUniform1f(ambient, ~ambient);
	glUniform1f(diffuse, ~diffuse);
	glUniform1f(specular, ~specular);
	glUniform1f(shininess, ~shininess);
}

void LitImage::loadImage(const char* imgFile) {
	texture = ImageUtil0::LoadImageTexture(imgFile);
}

void LitImage::initUniforms() {
	sharedUniforms.model(getLocation("model"));

	ambient(getLocation("ambientStrength"));
	diffuse(getLocation("diffStrength"));
	specular(getLocation("specularStrength"));
	shininess(getLocation("shininess"));
}