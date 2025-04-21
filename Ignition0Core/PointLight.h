/**
 * Author:	Varun Khambhata
 * Created: 17.01.2024
**/

#ifndef __POINTLIGHT__
#define __POINTLIGHT__

#include <Ignition0Core/Light0.h>

class PointLight: public Light0 {
private:
	float Radius, FallOff;

protected:
	void updateUBO(const int index);

public:
	PointLight();
	void setProperties(float radius, float fallOff);

	struct Properties {
		glm::vec4 position;
		glm::vec4 color;
		glm::vec4 properties;
	};
};

#endif