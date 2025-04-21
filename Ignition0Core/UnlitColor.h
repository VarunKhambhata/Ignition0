/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __UNLITCOLOR__
#define __UNLITCOLOR__

#include <Ignition0Core/Material0.h>

class UnlitColor: public Material0 {
private:
	UniformLink<float> red;
	UniformLink<float> green;
	UniformLink<float> blue;

protected:
	void onUsed() override;
	void initUniforms() override;
	std::string fragmentShaderSource() override;
	std::string vertexShaderSource() override;
	AttribNames bindShaderAttribs() override;

public:
	UnlitColor(float r, float g, float b);
};

#endif