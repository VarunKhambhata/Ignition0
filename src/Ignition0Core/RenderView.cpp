/**
 * Author:	Varun Khambhata
 * Created: 7.07.2024
**/

#include <Ignition0Core/RenderView.h>

RenderView::RenderView(const glm::mat4 &projection, const glm::vec3 &pos, const glm::vec3 &rot, unsigned int instances)
		: Projection(projection), Position(pos), Rotation(rot), Instances(instances) {}