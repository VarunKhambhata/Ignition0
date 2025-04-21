/**
 * Author:	Varun Khambhata
 * Created: 30.05.2024
**/

#ifndef __RENDERINFO__
#define __RENDERINFO__

#include <glm/glm.hpp>

struct RenderInfo {
	const glm::vec3 &CamPosition, &CamRotation;
	const unsigned int DrawInstances;

	RenderInfo(const glm::vec3 &camPos, const glm::vec3 &camRot, const unsigned int drawInstances = 1);
};

#endif