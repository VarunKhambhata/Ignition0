/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __UNLITCOLOR__
#define __UNLITCOLOR__

#include <Ignition0Core/Material0.h>

class UnlitColor: public Material0 {
private:
	GLint uniR, uniG, uniB;

public:
	struct {
		float r,g,b;
	} uniforms;

	std::string fragmentShaderSource();
	std::string vertexShaderSource();

	UnlitColor(float r, float g, float b);
	~UnlitColor();
	void onUsed();
	void initUniforms();
	void onUniformsUpdate();
};

#endif