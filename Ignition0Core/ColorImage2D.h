/**
 * Author:	Varun Khambhata
 * Created: 13.04.2025
**/

#ifndef __ColorImage2D__
#define __ColorImage2D__

#include <Ignition0Core/Material0.h>

class ColorImage2D: public Material0 {
private:
	GLuint texture;

protected:
	void onUsed() override;
	void setTexture(GLuint tex) override;
	std::string fragmentShaderSource() override;
	std::string vertexShaderSource() override;
	AttribNames bindShaderAttribs() override;	

public:
	ColorImage2D();
};

#endif