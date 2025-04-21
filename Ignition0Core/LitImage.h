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
	LitImage();	
	void setMaterialProperties(float ambient, float diffuse, float specular, float shininess = 32);
	void loadImage(const char* imgFile);
};

#endif