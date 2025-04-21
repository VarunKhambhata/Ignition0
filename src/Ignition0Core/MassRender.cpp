/**
 * Author:	Varun Khambhata
 * Created: 27.11.2024
**/

#include <GL/glew.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/MassRender.h>

MassRender::MassRender(): instances(1) {
	childVisible = false;
	glGenBuffers(1, &transformSSBO);
}

MassRender::~MassRender() {
	glDeleteBuffers(1, &transformSSBO);
}

void MassRender::setMassTransforms(const std::vector<TransformVecs> transforms) {
	instances = transforms.size();
	glm::mat4 tMats[instances];

	for(int i = 0; i < instances; i++) {
		tMats[i] = glm::translate(transforms[i].position)
					* glm::eulerAngleYXZ(glm::radians(transforms[i].rotation.y), -glm::radians(transforms[i].rotation.x), -glm::radians(transforms[i].rotation.z))
					* glm::scale(transforms[i].scale);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, transformSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * instances, tMats, GL_STATIC_DRAW);

	InternalIgnition0::enableInstanceMat4(InternalIgnition0::identityMat4SSBO);
}

void MassRender::onDraw(RenderInfo& rInfo) {
	RenderInfo rInfoMR(rInfo.CamPosition, rInfo.CamRotation, instances);

	std::vector<m<Object0>>::iterator it, end;
	childList(it, end);

	InternalIgnition0::enableInstanceMat4(transformSSBO);
	while(it != end)
		requestDraw(**it++, rInfoMR);
	InternalIgnition0::enableInstanceMat4(InternalIgnition0::identityMat4SSBO);
}