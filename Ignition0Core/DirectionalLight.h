/**
 * Author:	Varun Khambhata
 * Created: 17.07.2024
**/

#ifndef __DIRECTIONALLIGHT0__
#define __DIRECTIONALLIGHT0__

#include <Ignition0Core/Light0.h>

class DirectionalLight: public Light0 {
private:
	glm::vec3 Direction;

public:
	void updateUBO(const int index);
	void applyStateUpdate() override final;
};

#endif