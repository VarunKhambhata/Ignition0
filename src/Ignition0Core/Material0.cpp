/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Logger0.h>
#include <Ignition0Core/Material0.h>

struct ShaderContextTransparent {
	bool built = false;
	unsigned int shaderProgram = 0;
	unsigned int usageCount = 0;
};


unsigned int Material0::compileShader(std::string source, GLuint type) {
	unsigned int shader = glCreateShader(type);

	const char *sourceCS = source.c_str();

    glShaderSource(shader, 1, &sourceCS, NULL);
    glCompileShader(shader);

    int success;
	char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    Logger0E("ERROR: " << ((type == 35632)?"Fragment":"Vertex") << " Shader Compilation Failed\n\t" << infoLog);
	    return -1;
	};

    return shader;
}

unsigned int Material0::getShaderProgram() {
	return shaderProgram;
}

Material0::Material0() : registeredShaderContext(nullptr) {}

Material0::~Material0() {
	if(!registeredShaderContext) {
		destroy();
		return;
	}

	ShaderContextTransparent* registeredShaderContextT = reinterpret_cast<ShaderContextTransparent*>(registeredShaderContext);
	if(registeredShaderContextT->usageCount > 0) {
		registeredShaderContextT->usageCount --;
		if(registeredShaderContextT->built && registeredShaderContextT->usageCount == 0)
			destroy();
	}
}

Material0::BuildInfo Material0::build(ShaderContext* sdrCtx) {
	// common shader compiled once across all Material0 objects
	if(!commonVShaderLib) commonVShaderLib = compileShader(commonVShaderLibSource, GL_VERTEX_SHADER);
	if(!commonFShaderLib) commonFShaderLib = compileShader(commonFShaderLibSource, GL_FRAGMENT_SHADER);

	ShaderContextTransparent* registeredShaderContextT = nullptr;
	if(sdrCtx) {
		registeredShaderContext = sdrCtx;
		registeredShaderContextT = reinterpret_cast<ShaderContextTransparent*>(registeredShaderContext);
		registeredShaderContextT->usageCount ++;

		if(registeredShaderContextT->built) {
			shaderProgram = registeredShaderContextT->shaderProgram;
			initUniforms();
			return SHADER_BUILD_EXISTS;
		}
	}
	

	unsigned int vertexShader   = compileShader(vertexShaderSource(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(fragmentShaderSource(), GL_FRAGMENT_SHADER);

	if(vertexShader == -1 || fragmentShader == -1) {
		return SHADER_BUILD_ERROR;
	}


	shaderProgram = glCreateProgram();
	AttribNames attribs = bindShaderAttribs();	
	if(!attribs.vertex.empty())   glBindAttribLocation(shaderProgram, AttribLocation::VERTEX, attribs.vertex.c_str());
	if(!attribs.texCoord.empty()) glBindAttribLocation(shaderProgram, AttribLocation::TEXTURE, attribs.texCoord.c_str());
	if(!attribs.normal.empty())   glBindAttribLocation(shaderProgram, AttribLocation::NORMAL, attribs.normal.c_str());
	glAttachShader(shaderProgram, commonVShaderLib);
    glAttachShader(shaderProgram, commonFShaderLib);
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    initShaderStorage();
    initUniforms();

	if(registeredShaderContextT != nullptr) {
		registeredShaderContextT->built = true;
		registeredShaderContextT->shaderProgram = shaderProgram;
	}
	return SHADER_BUILD_OK;
}

void Material0::initShaderStorage() {
	GLuint camBlockIndex = glGetUniformBlockIndex(shaderProgram, "CameraData");
    if(camBlockIndex != GL_INVALID_INDEX) {
    	glUniformBlockBinding(shaderProgram, camBlockIndex, Scene::BufferBinding::CAMERA_VIEW);
    }

	GLuint instTransfromIndex = glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "InstancedTransforms");
    if(instTransfromIndex != GL_INVALID_INDEX) {
    	glShaderStorageBlockBinding(shaderProgram, instTransfromIndex, Scene::BufferBinding::INSTANCED_TRANSFORM);
    }

	GLuint drlBlockIndex = glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "DirectionalLights");
    if(drlBlockIndex != GL_INVALID_INDEX) {
    	glShaderStorageBlockBinding(shaderProgram, drlBlockIndex, Scene::BufferBinding::DIRECTIONAL_LIGHT);
    }

	GLuint ptlBlockIndex = glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "PointLights");
    if(ptlBlockIndex != GL_INVALID_INDEX) {
    	glShaderStorageBlockBinding(shaderProgram, ptlBlockIndex, Scene::BufferBinding::POINT_LIGHTS);
    }
}

void Material0::destroy() {
	glDeleteProgram(shaderProgram);
}

void Material0::use() {
	glUseProgram(shaderProgram);
	updateSharedUniforms();
	onUsed();
}

GLint Material0::getLocation(const char* s) { 
	return glGetUniformLocation(shaderProgram, s);
}

void Material0::updateSharedUniforms() {
	if(sharedUniforms.model.valueUpdated)
		glUniformMatrix4fv(sharedUniforms.model, 1, GL_FALSE, glm::value_ptr(* ~sharedUniforms.model));
}







// UniformLink class implementations ***************************************
template class Material0::UniformLink<float>;
template class Material0::UniformLink<glm::vec3>;
template class Material0::UniformLink<glm::mat4*>;


template<typename T>
void Material0::UniformLink<T>::operator()(GLint loc) {
	location = loc;
	hasLocation = true;
}


template<typename T>
void Material0::UniformLink<T>::operator=(T val) {
	value = val;
	valueUpdated = true;
}

template<typename T>
Material0::UniformLink<T>::operator GLint() const {
	return location;
}

template<typename T>
T Material0::UniformLink<T>::operator~(){
	valueUpdated = false;
	return value;
}
// *************************************************************************

// shared shader library implementation ************************************
unsigned int Material0::commonVShaderLib = 0;
std::string Material0::commonVShaderLibSource = R"(
	#version 430 core
	layout (packed) uniform CameraData {
		mat4 projection;
		mat4 view;
		vec3 position;
	};

	layout(packed) readonly buffer InstancedTransforms {
		mat4 iTransfroms[];
	};

	mat4 getView() { return view; }
	mat4 getProjection() { return projection; }
	vec3 getCamPosition() { return position; }

	mat4 getInstancedTransfrom() { return iTransfroms[gl_InstanceID]; }
)";

unsigned int Material0::commonFShaderLib = 0;
std::string Material0::commonFShaderLibSource = R"(
	#version 430 core

	struct DirectionalLightProp {
		vec3 direction;
		vec4 properties;
	};

	struct PointLightProp {
		vec3 position;
		vec3 color;
		vec3 properties;
	};

	layout(packed) readonly buffer DirectionalLights {
		int totalDrLights;
		DirectionalLightProp drLights[];
	};

	layout(packed) readonly buffer PointLights {
		int totalPtLights;
		PointLightProp ptLights[];
	};

	int getMaxDirectionalLights() { return totalDrLights; }
	vec3 getDirectionalLight(int index) { return drLights[index].direction; }
	vec3 getDirectionalLightColor(int index) { return drLights[index].properties.rgb; }
	float getDirectionalLightIntensity(int index) {	return drLights[index].properties.a; }

	int getMaxPointLights() { return totalPtLights; }
	vec3 getLightPosition(int index) { return ptLights[index].position; }
	vec3 getLightColor(int index) {	return ptLights[index].color;	}
	float getLightRadius(int index) { return ptLights[index].properties.x; }
	float getLightIntensity(int index) { return ptLights[index].properties.y; }
	float getLightFallOff(int index) { return ptLights[index].properties.z; }

	vec3 DirectionalLightEffect(int index, vec3 viewDir, vec3 norm, float ambientStrength, float diffStrength, float specularStrength, float shininess) {
		vec3 lColor = getDirectionalLightColor(index);
		vec3 lDirection = getDirectionalLight(index);

		// ambient
		vec3 ambient = lColor * ambientStrength;
		
		// diffuse
		float diffuseStrength = max(dot(norm, lDirection), 0.0);
		vec3 diffuse = lColor * (diffuseStrength * diffStrength);

		// specular :Blinn-Phong
		vec3 halfwayDir = normalize(lDirection + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
		vec3 specular = lColor * (spec * specularStrength);

		// attenuation
		return (ambient + diffuse + specular) * getDirectionalLightIntensity(index);
	}

	vec3 PointLightEffect(int index, vec3 viewDir, vec3 norm, vec3 FragPos, float ambientStrength, float diffStrength, float specularStrength, float shininess) {
		// ambient
		vec3 ambient = ptLights[index].color * ambientStrength;

		// diffuse
		vec3 lightDir = normalize(ptLights[index].position - FragPos);
		float diffuseStrength = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = ptLights[index].color * (diffuseStrength * diffStrength);

		// specular :Blinn-Phong
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
		vec3 specular = ptLights[index].color * (spec * specularStrength);

		// attenuation
		float distance = length(ptLights[index].position - FragPos);
		float attenuation; {
			//attenuate_cusp
			float s = min(distance / getLightRadius(index), 1.0);
			float s2 = pow(s,2);
			attenuation = getLightIntensity(index) * pow(1 - s2,2) / (1 + getLightFallOff(index) * s);
		}

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		return (ambient + diffuse + specular);
	}
)";
// *************************************************************************