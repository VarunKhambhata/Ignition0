/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __UNLITIMAGE__
#define __UNLITIMAGE__

#include <Ignition0Core/Material0.h>

class UnlitImage: public Material0 {
private:
	GLuint texture;

public:
	std::string fragmentShaderSource();
	std::string vertexShaderSource();

	UnlitImage(bool init = true);
	void onUsed();
	void setTexture(GLuint tex);
	void setTexture(const char* imgFile);
	void initUniforms();
};

#endif