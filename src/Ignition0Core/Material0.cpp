/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Ignition0Core/Logger0.h>

#include <Ignition0Core/Material0.h>
#include <Ignition0Core/InternalIgnition0.h>

unsigned int Material0::compileShader(std::string source, GLuint type) {
	unsigned int shader = glCreateShader(type);

	const char *sourceCS = source.c_str();

    glShaderSource(shader, 1, &sourceCS, NULL);
    glCompileShader(shader);

    int success;
	char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
	{
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    Logger0E("ERROR: " << ((type == 35632)?"Fragment":"Vertex") << " Shader Compilation Failed\n\t" << infoLog);
	    return -1;
	};

    return shader;
}

void Material0::build() {
	unsigned int vertexShader   = compileShader(vertexShaderSource(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(fragmentShaderSource(), GL_FRAGMENT_SHADER);

	if(vertexShader == -1 || fragmentShader == -1){
		return;
	}

	shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    initUniforms();
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
	if(sharedUniforms.projection_pair.second) {
		glUniformMatrix4fv(sharedUniforms.projection_pair.first, 1, GL_FALSE, glm::value_ptr(*sharedUniforms.projection_pair.second));
	}
}