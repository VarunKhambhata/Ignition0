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

protected:
	void onUsed() override;
	void initUniforms() override;
	std::string fragmentShaderSource() override;
	std::string vertexShaderSource() override;
	AttribNames bindShaderAttribs() override;

public:
	UnlitImage();
	~UnlitImage();
	void loadImage(const char* imgFile);
};

#endif