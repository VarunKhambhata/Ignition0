/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Core/Scene.h>

Scene::Scene(): screen(Plane()) {
	switch(internal::Ignition0.preferedDetail()) {
		case Detail::LOW:    directLightBuffer.size = pointLightBuffer.size = MAX_LIGHT/64; break;
		case Detail::MEDIUM: directLightBuffer.size = pointLightBuffer.size = MAX_LIGHT/16; break;
		case Detail::HIGH:   directLightBuffer.size = pointLightBuffer.size = MAX_LIGHT/02; break;
		case Detail::ULTRA:  directLightBuffer.size = pointLightBuffer.size = MAX_LIGHT/01;	break;
	}

	int initSize = 0;

	glGenBuffers(1, &directLightBuffer.bufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, directLightBuffer.bufferObject);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * 4 + sizeof(Material0::DirectionalLightProperties) * directLightBuffer.size, nullptr, GL_DYNAMIC_DRAW);	// Allocate buffer memory
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int) * 4, &initSize);
	glBindBufferBase(GL_UNIFORM_BUFFER, UboBinding::DIRECTIONAL_LIGHT, directLightBuffer.bufferObject);	// Bind the buffer to the binding point

	glGenBuffers(1, &pointLightBuffer.bufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, pointLightBuffer.bufferObject);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * 4 + sizeof(Material0::PointLightProperties) * pointLightBuffer.size, nullptr, GL_DYNAMIC_DRAW);	// Allocate buffer memory
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int) * 4, &initSize);
	glBindBufferBase(GL_UNIFORM_BUFFER, UboBinding::POINT_LIGHTS, pointLightBuffer.bufferObject);	// Bind the buffer to the binding point
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Scene::~Scene() {
	glDeleteBuffers(1, &directLightBuffer.bufferObject);
	glDeleteBuffers(1, &pointLightBuffer.bufferObject);
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

void Scene::syncLights() {
	pointLightBuffer.resetHead();
	directLightBuffer.resetHead();

	glBindBuffer(GL_UNIFORM_BUFFER, directLightBuffer.bufferObject);
	if(directLightBuffer.needsUpdate) {
		directLightBuffer.needsUpdate = false;
		int size = DirectionalLights.size() < directLightBuffer.size ? DirectionalLights.size() : directLightBuffer.size;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int) * 4, &size);
	}

	for(m<Light0> light: DirectionalLights) {
		light->updateLight(directLightBuffer);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, pointLightBuffer.bufferObject);
	if(pointLightBuffer.needsUpdate) {
		pointLightBuffer.needsUpdate = false;
		int size = PointLights.size() < pointLightBuffer.size ? PointLights.size() : pointLightBuffer.size;;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int) * 4, &size);
	}

	for(m<Light0> light: PointLights)
		light->updateLight(pointLightBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
	PointLights.push_back(light);
	pointLightBuffer.needsUpdate = true;
}

void Scene::add(m<DirectionalLight> light) {
	Objects.push_back(light);
	DirectionalLights.push_back(light);
	directLightBuffer.needsUpdate = true;
}

void Scene::update() {
	uint8_t STATE = 0;

	for(m<Object0> obj: Objects)
		STATE |= obj->update(internal::Ignition0.IDENTITY);

	syncLights();

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

    // Logger0(depthValue <<"\t- " <<(int)color[0] <<" " <<(int)color[1] <<" " <<(int)color[2] <<" [" <<attachmentStatus1 << " " << attachmentStatus2<<"]");
}
