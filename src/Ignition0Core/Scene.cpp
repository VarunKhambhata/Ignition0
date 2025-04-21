/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/Scene.h>

Scene::Scene() {
	switch(InternalIgnition0::preferedDetail()) {
		case Detail::LOW:    directLightBuffer.size = pointLightBuffer.size = InternalIgnition0::MAX_LIGHT/64; break;
		case Detail::MEDIUM: directLightBuffer.size = pointLightBuffer.size = InternalIgnition0::MAX_LIGHT/16; break;
		case Detail::HIGH:   directLightBuffer.size = pointLightBuffer.size = InternalIgnition0::MAX_LIGHT/02; break;
		case Detail::ULTRA:  directLightBuffer.size = pointLightBuffer.size = InternalIgnition0::MAX_LIGHT/01; break;
	}

	int initSize = 0;
	// Directional Lights UBO
	glGenBuffers(1, &directLightBuffer.bufferObject);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, directLightBuffer.bufferObject);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 4 + sizeof(DirectionalLight::Properties) * directLightBuffer.size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int) * 4, &initSize);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BufferBinding::DIRECTIONAL_LIGHT, directLightBuffer.bufferObject);
	// Point Lights UBO
	glGenBuffers(1, &pointLightBuffer.bufferObject);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightBuffer.bufferObject);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 4 + sizeof(PointLight::Properties) * pointLightBuffer.size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int) * 4, &initSize);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BufferBinding::POINT_LIGHTS, pointLightBuffer.bufferObject);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

Scene::~Scene() {
	glDeleteBuffers(1, &directLightBuffer.bufferObject);
	glDeleteBuffers(1, &pointLightBuffer.bufferObject);
}

void Scene::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::drawFrame(Camera &cam) {
	static RenderInfo rInfo(glm::vec3(0), glm::vec3(0), 1);
	m<Material0> mat = cam.getMaterial();
	mat->setTexture(cam.getRenderedTexture());
	InternalIgnition0::plane->setMaterial(mat);
	InternalIgnition0::plane->draw(rInfo);
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

	for(m<Light0> light: DirectionalLights)
		light->updateLight(directLightBuffer);


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

bool Scene::update() {
	uint8_t STATE = 0;

	for(m<Object0> obj: Objects)
		STATE |= obj->update(InternalIgnition0::IDENTITY);

	if(!((STATE | directLightBuffer.needsUpdate | pointLightBuffer.needsUpdate)))
		return false;

	clear();
	syncLights();
	Object0::drawAllowed = true;
	for(m<Camera> cam: Cameras) {
		RenderInfo rInfo(cam->getPosition(), cam->getRotation(), 1);

		cam->open();
		for(m<Object0> obj: Objects)
			obj->draw(rInfo);

		cam->visible = true;
		cam->draw(rInfo);
		cam->visible = false;
		cam->close();
		drawFrame(*cam);
	}
	Object0::drawAllowed = false;
	return true;
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
