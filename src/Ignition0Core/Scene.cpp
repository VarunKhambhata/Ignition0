/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Logger0.h>

Scene::Scene(): screen(Plane()) {
	glGenBuffers(1, &lightArrayBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightArrayBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * 4 + sizeof(Material0::LightProperties) * Material0::LightProperties::MAX_LIGHTS, nullptr, GL_DYNAMIC_DRAW);	// Allocate buffer memory
	int size = 0; glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int) * 4, &size);
	glBindBufferBase(GL_UNIFORM_BUFFER, UboBinding::POINT_LIGHTS, lightArrayBuffer);	// Bind the buffer to the binding point
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Scene::~Scene() {
	glDeleteBuffers(1, &lightArrayBuffer);
}

void Scene::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::updateFrame(Camera &cam) {
	cam.close();

	const RenderView rvScreen(internal::Ignition0.IDENTITY, internal::Ignition0.ORIGIN, internal::Ignition0.ORIGIN);

	m<UnlitImage> mat = std::static_pointer_cast<UnlitImage>(cam.getMaterial());
	mat->setTexture(cam.getRenderedTexture());
	screen.setMaterial(mat);
	screen.draw(rvScreen);
}

void Scene::add(m<Object0> obj) {
	Objects.push_back(obj);
}

void Scene::add(m<Camera> obj) {
	Objects.push_back(obj);
	Cameras.push_back(obj);
}

void Scene::add(m<PointLight> light) {
	Objects.push_back(light);
	Lights.push_back(light);
	int size = Lights.size();

	glBindBuffer(GL_UNIFORM_BUFFER, lightArrayBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int) * 4, &size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Scene::update() {
	uint8_t STATE = 0;

	for(m<Object0> obj: Objects)
		STATE |= obj->update(internal::Ignition0.IDENTITY);

	if(STATE) {
		int lightIndex = 0;
		for(m<PointLight> light: Lights) {
			if(lightIndex < Material0::LightProperties::MAX_LIGHTS)
				light->updateUBO(lightArrayBuffer, lightIndex);
			lightIndex ++;
		}
	}


	clear();
	for(m<Camera> cam: Cameras) {
		if(STATE) {
			const glm::mat4 viewProj = cam->getProjection() * cam->getGlobalTransformation();
			const glm::mat4 uiProj   = glm::scale(cam->getProjection(), glm::vec3(-1,1,-1));
			const glm::vec3 camPos   = cam->getPosition();
			const glm::vec3 camRot   = cam->getRotation();
			
			const RenderView rv3D(viewProj, camPos, camRot);
			const RenderView rvUI(uiProj, camPos, camRot);

			cam->open();
			for(m<Object0> obj: Objects)
				obj->draw(rv3D);

			cam->visible = true;
			cam->draw(rvUI);
			cam->visible = false;
		}
		updateFrame(*cam);
	}
}

void Scene::resize() {
	for(m<Camera> cam: Cameras)
		cam->reload();
}

void Scene::pickCenterPixel(glm::vec2 sz) {
	GLubyte color[3] = {0,0,0};
	GLfloat depthValue;
    glReadPixels(sz.x/2, sz.y/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);
    glReadPixels(sz.y/2, sz.y/2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color);
    GLint attachmentStatus1,attachmentStatus2;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachmentStatus1);
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachmentStatus2);

    Logger0(depthValue <<"\t- " <<(int)color[0] <<" " <<(int)color[1] <<" " <<(int)color[2] <<" [" <<attachmentStatus1 << " " << attachmentStatus2<<"]");
}
