/**
 * Author:	Varun Khambhata
 * Created: 30.05.2024
**/

#ifndef __RENDERVIEW__
#define __RENDERVIEW__

#include <glm/glm.hpp>

struct RenderView {
	const glm::mat4 &Projection;
	const glm::vec3 &Position, &Rotation;
	const unsigned int Instances;

	RenderView(const glm::mat4 &projection, const glm::vec3 &pos, const glm::vec3 &rot, const unsigned int instances = 1);
};

#endif