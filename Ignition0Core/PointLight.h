/**
 * Author:	Varun Khambhata
 * Created: 25.10.2023
**/

#ifndef __POINTLIGHT__
#define __POINTLIGHT__

#include <Ignition0Core/Object0.h>

class PointLight: public Object0 {
private:
	glm::vec4 Color;
	float Radius, Intensity, FallOff;

public:
	PointLight();
	void setColor(float r, float g, float b);
	void setProperties(float radius, float intensity, float fallOff);
	void updateUBO(GLuint lpArrayBuffer, int index);
};

#endif