/**
 * Author:	Varun Khambhata
 * Created: 12.01.2024
**/

#ifndef __LITIMAGE__
#define __LITIMAGE__

#include <Ignition0Core/Material0.h>

class LitImage: public Material0 {
private:
	GLuint texture;
	GLint uniAmbient, uniDiffuse, uniSpecular;

public:
	struct {
		UniformLink<float> ambient;
		UniformLink<float> diffuse;
		UniformLink<float> specular;
		UniformLink<float> shininess;
	} uniforms;

	std::string fragmentShaderSource();
	std::string vertexShaderSource();

	LitImage(bool init = true);
	void onUsed();
	void setTexture(GLuint tex);
	void setMaterialProperties(float ambient, float diffuse, float specular, float shininess = 32);
	void setTexture(const char* imgFile);
	void initUniforms();
	void onUniformsUpdate();
};

#endif