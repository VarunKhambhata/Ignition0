/**
 * Author:	Varun Khambhata
 * Created: 25.10.2023
**/

#ifndef __LITCOLOR__
#define __LITCOLOR__

#include <Ignition0Core/Material0.h>

class LitColor: public Material0 {
private:
	UniformLink<glm::vec3> color;
	UniformLink<float> ambient;
	UniformLink<float> diffuse;
	UniformLink<float> specular;
	UniformLink<float> shininess;

protected:
	void onUsed() override;
	void initUniforms() override;
	std::string fragmentShaderSource() override;
	std::string vertexShaderSource() override;
	AttribNames bindShaderAttribs() override;	

public:
	LitColor();
	void setMaterialProperties(float r, float g, float b, float ambient, float diffuse, float specular, float shininess = 32);
};

#endif