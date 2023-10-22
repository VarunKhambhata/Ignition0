/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __MATERIAL0__
#define __MATERIAL0__

#include <GL/gl.h>

class Material0 {
private:
	unsigned int shaderProgram;
	static unsigned int compileShader(std::string source, GLuint type);

protected:
	unsigned int getShaderProgram();
	void setShaderProgram(unsigned int program);

public:
	union {
		std::pair<GLint, glm::mat4*> projection_pair  = std::make_pair(-1, nullptr);
		void projection(GLint loc)      { projection_pair.first =  loc; }
		void projection(glm::mat4* val) { projection_pair.second = projection_pair.first != -1? val: nullptr; }

	} sharedUniforms;

	virtual std::string vertexShaderSource() = 0;
	virtual std::string fragmentShaderSource() = 0;
	virtual void 		initUniforms() {};
	virtual void 		onUniformsUpdate() {};
	virtual void 		onUsed() {};

	void build();
	void destroy();
	void use();

	GLint getLocation(const char* s);

	void updateUniforms();
	void updateSharedUniforms();
};

#endif