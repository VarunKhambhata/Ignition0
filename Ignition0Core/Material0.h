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
	static unsigned int commonShaderLib;
	static std::string commonShaderLibSource;
	static unsigned int compileShader(std::string source, GLuint type);
	void initInternalUniforms();


protected:
	unsigned int getShaderProgram();
	void setShaderProgram(unsigned int program);

	template <class T>
	class UniformLink {
	private:
		T value;
		GLint location;
	public:
		bool hasLocation  = false;
		bool valueUpdated = false;

		void operator()(GLint loc);
		void operator=(T val);
		operator GLint() const;
		T operator~();
	};

public:
	struct {
		UniformLink<glm::mat4*> mvp;
		UniformLink<glm::mat4*> model;
		UniformLink<glm::vec3>  camPosition;
	} sharedUniforms;

	struct PointLightProperties {
		glm::vec4 position; 
		glm::vec4 color;
		glm::vec4 properties;
	};

	struct DirectionalLightProperties {
		glm::vec4 direction;
		glm::vec4 properties;
	};

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
