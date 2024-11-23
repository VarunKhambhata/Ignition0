/**
 * Author:	Varun Khambhata
 * Created: 17.01.2024
**/

#include <GL/glew.h>

#include <Ignition0Core/PointLight.h>
#include <Ignition0Core/Material0.h>

void PointLight::setProperties(float radius, float fallOff) { Radius = radius; FallOff = fallOff;  PENDING_STATE |= PROPERTY_CHANGED;}

PointLight::PointLight(): Radius(10), FallOff(1) {}

void PointLight::updateUBO(const int index) {
	if(STATE) {
		Material0::PointLightProperties data = {
			glm::vec4(getPosition(), 0),
			glm::vec4(Color, 0),
			glm::vec4(Radius, Intensity, FallOff, 0)
		};

		glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(int) * 4) + (sizeof(Material0::PointLightProperties) * index), sizeof(Material0::PointLightProperties), &data);
	}
}