/**
 * Author:	Varun Khambhata
 * Created: 25.10.2023
**/

#ifndef __LITCOLOR__
#define __LITCOLOR__

#include <Ignition0Core/Material0.h>

class LitColor: public Material0 {
private:
	GLint uniR, uniG, uniB;
	GLint uniAmbient, uniDiffuse, uniSpecular;

public:	
	struct {
		UniformLink<glm::vec3> color;
		UniformLink<float> ambient;
		UniformLink<float> diffuse;
		UniformLink<float> specular;
		UniformLink<float> shininess;
	} uniforms;


	std::string fragmentShaderSource();
	std::string vertexShaderSource();

	LitColor(float r, float g, float b, float ambient, float diffuse, float specular, float shininess = 32);

	void onUsed();
	void initUniforms();
	void onUniformsUpdate();
};

#endif