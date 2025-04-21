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

protected:
	void updateUBO(const int index) override;

public:
	void applyStateUpdate() override final;

	struct Properties {
		glm::vec4 direction;
		glm::vec4 properties;
	};
};

#endif