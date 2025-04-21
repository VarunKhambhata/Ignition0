/**
 * Author:	Varun Khambhata
 * Created: 7.07.2024
**/

#include <Ignition0Core/RenderInfo.h>

RenderInfo::RenderInfo(const glm::vec3 &camPos, const glm::vec3 &camRot, unsigned int drawInstances)
			: CamPosition(camPos), CamRotation(camRot), DrawInstances(drawInstances) {}