/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __SCENE__
#define __SCENE__

#include <Ignition0Core/Object0.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/Plane.h>

class Scene {
private:
	std::vector<m<Object0>> RenderObjects;
	std::vector<m<Camera>> Cameras;
	Plane screen;
	glm::mat4 Projection;
	glm::ivec2 drawSize;

	void clear();
	void updateFrame(Camera &c);

public:
	Scene();
	void add(m<Object0> obj);
	void add(m<Camera> cam);
	void update();
	void resize();
	void pickCenterPixel(glm::vec2 sz);
};

#endif