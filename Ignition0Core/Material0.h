/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __MATERIAL0__
#define __MATERIAL0__

#include <GL/gl.h>
#include <glm/glm.hpp>

#include <string>

struct ShaderContext { private: uint8_t _[12]; };
enum AttribLocation { VERTEX  = 0, TEXTURE = 1, NORMAL  = 2 };
struct AttribNames { std::string vertex; std::string texCoord; std::string normal; };

class Material0 {
private:
	unsigned int shaderProgram;
	ShaderContext* registeredShaderContext;
	static unsigned int commonVShaderLib, commonFShaderLib;
	static std::string commonVShaderLibSource, commonFShaderLibSource;

	static unsigned int compileShader(std::string source, GLuint type);
	void 				initShaderStorage();

protected:
	enum BuildInfo { SHADER_BUILD_OK, SHADER_BUILD_EXISTS, SHADER_BUILD_ERROR };

	virtual std::string vertexShaderSource() = 0;
	virtual std::string fragmentShaderSource() = 0;
	virtual AttribNames bindShaderAttribs() { return {}; };
	virtual void 		onUsed() {};
	virtual void 		initUniforms() {};
	unsigned int 		getShaderProgram();
	void		 		updateSharedUniforms();
	BuildInfo 			build(ShaderContext* sdrCtx = nullptr);
	void 				destroy();

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
	Material0();
	~Material0();

	GLint 		 getLocation(const char* s);
	void 		 use();
	virtual void setTexture(GLuint tex) {};

	struct {
		UniformLink<glm::mat4*> model;
	} sharedUniforms;
};

#endif