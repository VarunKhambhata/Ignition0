/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __INTERNALIGNITION0__
#define __INTERNALIGNITION0__

#include <glm/glm.hpp>

#include <Ignition0Core/Ignition0Environment.h>
#include <Ignition0Core/Object0.h>
#include <Ignition0Core/UnlitColor.h>
#include <Ignition0Core/ColorImage2D.h>

namespace InternalIgnition0 {
	extern const glm::mat4 IDENTITY;
	extern const glm::vec3 ORIGIN;

	extern m<UnlitColor> missing;
	extern m<ColorImage2D> colorImage;
	extern m<Object0> plane;

	extern const unsigned int MAX_LIGHT;
	extern GLuint identityMat4SSBO;

	glm::ivec2 displaySize();
	Detail 	   preferedDetail();
	float	   deltaTime();
	void	   enableInstanceMat4(GLuint &ssbo);
}

#endif