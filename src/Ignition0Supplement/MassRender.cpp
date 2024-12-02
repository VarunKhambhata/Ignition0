/**
 * Author:	Varun Khambhata
 * Created: 27.11.2024
**/

#include <GL/glew.h>
#include <glm/gtx/euler_angles.hpp>

#include <Ignition0Core/Scene.h>
#include <Ignition0Supplement/MassRender.h>

MassRender::MassRender(): instances(1) {
	childVisible = false;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
}

MassRender::~MassRender() {
	glDeleteBuffers(1, &instanceVBO);
}

void MassRender::applyStateUpdate() {
	Object0::applyStateUpdate();
	
	if(PENDING_STATE & CHILD_ADDED) {
		std::vector<m<Object0>>::iterator it, end;
		childList(it, end);

		while(it != end) {
			(*it++)->extend(&MassRender::callback, this);
		}
	}
}

void MassRender::setMassTransforms(const std::vector<transformation> transforms) {
	instances = transforms.size();

	glm::mat4* tMats = new glm::mat4[instances];

	for(int i = 0; i < instances; i++) {
		float pitch = glm::radians(transforms[i].posX);
		float yaw   = glm::radians(transforms[i].posY);
		float roll  = glm::radians(transforms[i].posZ);
		
		tMats[i] = glm::translate(glm::vec3(transforms[i].posX, transforms[i].posY, transforms[i].posZ))
							 * glm::eulerAngleYXZ(transforms[i].rotY, -transforms[i].rotX, -transforms[i].rotZ) 
							 * glm::scale(glm::vec3(transforms[i].scaleX, transforms[i].scaleY, transforms[i].scaleZ));
	}

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, tMats, GL_STATIC_DRAW);
	delete tMats;
}


void MassRender::callback(const void* self) {
	((MassRender*)self)->enableVertexArray();
}

void MassRender::enableVertexArray() {
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	for (unsigned int i = 0; i < 4; i++) {
		  glEnableVertexAttribArray(5 + i);
		  glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (const GLvoid *)(sizeof(GLfloat) * i * 4));
		  glVertexAttribDivisor(5 + i, 1);
	}
}

void MassRender::onDraw(const RenderView& rView) {
	RenderView mrView(rView.Projection, rView.Position, rView.Rotation, instances);

	std::vector<m<Object0>>::iterator it, end;
	childList(it, end);
	while(it != end) {
		requestDraw(**it++, mrView);
	}
}
