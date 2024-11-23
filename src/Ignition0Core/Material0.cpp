/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/Material0.h>
#include <Ignition0Core/Logger0.h>
#include <Ignition0Core/Scene.h>

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

void Material0::setShaderProgram(unsigned int program) {
	shaderProgram = program;
}

void Material0::build() {
	if(!commonShaderLib)
		commonShaderLib = compileShader(commonShaderLibSource, GL_FRAGMENT_SHADER);

	unsigned int vertexShader   = compileShader(vertexShaderSource(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(fragmentShaderSource(), GL_FRAGMENT_SHADER);

	if(vertexShader == -1 || fragmentShader == -1)
		return;
	

	shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, commonShaderLib);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    initInternalUniforms();
    initUniforms();
}

void Material0::initInternalUniforms() {
	GLuint drlBlockIndex = glGetUniformBlockIndex(shaderProgram, "DirectionalLights");
    if(drlBlockIndex >= 0) {
    	// Associate the buffer with the buffer's binding point
    	// Actual buffer is bound to this binding point
    	glUniformBlockBinding(shaderProgram, drlBlockIndex, Scene::UboBinding::DIRECTIONAL_LIGHT);
    }

	GLuint ptlBlockIndex = glGetUniformBlockIndex(shaderProgram, "PointLights");
    if(ptlBlockIndex >= 0) {
    	// Associate the buffer with the buffer's binding point
    	// Actual buffer is bound to this binding point
    	glUniformBlockBinding(shaderProgram, ptlBlockIndex, Scene::UboBinding::POINT_LIGHTS);
    }   
}

void Material0::destroy() {
	glDeleteProgram(shaderProgram);	
}


void Material0::use() {
	glUseProgram(shaderProgram);
	onUsed();
}

GLint Material0::getLocation(const char* s) { 
	return glGetUniformLocation(shaderProgram, s);
}

void Material0::updateUniforms() {
	onUniformsUpdate();
}

void Material0::updateSharedUniforms() {
	if(sharedUniforms.mvp.valueUpdated)
		glUniformMatrix4fv(sharedUniforms.mvp, 1, GL_FALSE, glm::value_ptr(* ~sharedUniforms.mvp));

	if(sharedUniforms.model.valueUpdated)
		glUniformMatrix4fv(sharedUniforms.model, 1, GL_FALSE, glm::value_ptr(* ~sharedUniforms.model));

	if(sharedUniforms.camPosition.valueUpdated)
		glUniform3fv(sharedUniforms.camPosition, 1, & (~sharedUniforms.camPosition)[0]);
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


#define STR_EXPAND(x) #x
#define STR(x) STR_EXPAND(x)

// shared shader library implementation ************************************
unsigned int Material0::commonShaderLib = 0;
std::string Material0::commonShaderLibSource = R"(
	#version 330 core
	#define MAX_LIGHTS )" STR(MAX_LIGHT) R"(

	struct DirectionalLightProp {
		vec3 direction;
		vec4 properties;
	};

	struct PointLightProp {
		vec3 position;
		vec3 color;
		vec3 properties;
	};

	layout(packed) uniform DirectionalLights {
		int totalDrLights;
		DirectionalLightProp drLights[MAX_LIGHTS];
	};
	
	layout(packed) uniform PointLights {
		int totalPtLights;
		PointLightProp ptLights[MAX_LIGHTS];
	};

	int getMaxDirectionalLights() { return totalDrLights; }
	vec3 getDirectionalLight(int index) { return drLights[index].direction; }
	vec3 getDirectionalLightColor(int index) { return drLights[index].properties.rgb; }
	float getDirectionalLightIntensity(int index) {	return drLights[index].properties.a; }

	int getMaxLights() { return totalPtLights; }
	vec3 getLightPosition(int index) { return ptLights[index].position; }
	vec3 getLightColor(int index) {	return ptLights[index].color;	}
	float getLightRadius(int index) { return ptLights[index].properties.x; }
	float getLightIntensity(int index) { return ptLights[index].properties.y; }
	float getLightFallOff(int index) { return ptLights[index].properties.z; }
)";
// *************************************************************************


#undef STR_EXPAND
#undef STR