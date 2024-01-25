/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __SCENE__
#define __SCENE__

#include <Ignition0Core/Object0.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/Plane.h>
#include <Ignition0Core/PointLight.h>

class Scene {
private:
	std::vector<m<Object0>> RenderObjects;
	std::vector<m<Camera>> Cameras;
	std::vector<m<PointLight>> Lights;
	GLuint lightArrayBuffer;

	Plane screen;
	glm::mat4 Projection;
	glm::ivec2 drawSize;

	static m<Camera> currentCamera;

	void clear();
	void updateFrame(Camera &c);

public:
	Scene();
	~Scene();
	void add(m<Object0> obj);
	void add(m<Camera> cam);
	void add(m<PointLight> light);
	void update();
	void resize();
	void pickCenterPixel(glm::vec2 sz);


	static m<Camera> getCurrentCamera();

	enum UboBinding { POINT_LIGHTS = 1 };
};

#endif