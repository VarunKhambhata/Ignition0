/**
 * Author:	Varun Khambhata
 * Created: 17.07.2024
**/

#include <GL/glew.h>
#include <Ignition0Core/DirectionalLight.h>

void DirectionalLight::applyStateUpdate() {
	Light0::applyStateUpdate();

	if(PENDING_STATE & ROTATION_CHANGED) {
		Direction.y = sin(glm::radians(Rotation.x));
		Direction.x = cos(glm::radians(Rotation.x)) * cos(glm::radians(Rotation.y));
		Direction.z = cos(glm::radians(Rotation.x)) * sin(glm::radians(Rotation.y));
		Direction = glm::normalize(Direction);
	}
}

void DirectionalLight::updateUBO(const int index) {
	if(STATE & (ROTATION_CHANGED | PROPERTY_CHANGED)) {
		Properties data = {
			glm::vec4(Direction,0),
			glm::vec4(Color,Intensity)
		};

		glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(int) * 4) + (sizeof(Properties) * index), sizeof(Properties), &data);
	}
}