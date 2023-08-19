/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Logger0.h>

Scene::Scene(): screen(Plane()) {}

void Scene::clear() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

void Scene::updateFrame(Camera &c) {
	glm::vec2 size = c.getDisplaySize();
	glm::vec2 pos = c.getDisplayPosition();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(pos.x, pos.y, size.x, size.y);
	glEnable(GL_DEPTH_TEST);

	m<UnlitImage> mat = std::static_pointer_cast<UnlitImage>(c.getMaterial());
	mat->setTexture(c.getRenderedTexture());
	screen.setMaterial(mat);
	screen.update(proj);
}

void Scene::add(m<Object0> obj) {
	RenderObjects.push_back(obj);
}

void Scene::add(m<Camera> obj) {
	Cameras.push_back(obj);
}

void Scene::update() {
	clear();
	for(m<Camera> cam: Cameras) {
		cam->open();
		cam->update();
		glm::mat4 proj = cam->getProjection() * cam->getTransformation();
		for(m<Object0> obj: RenderObjects) {
			obj->update(proj);
		}
		updateFrame(*cam.get());
	}
}

void Scene::resize() {
	for(m<Camera> cam: Cameras) {
		cam->reload();
	}
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