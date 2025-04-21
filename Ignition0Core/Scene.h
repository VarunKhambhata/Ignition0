/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __SCENE__
#define __SCENE__

#include <Ignition0Core/Object0.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/DirectionalLight.h>
#include <Ignition0Core/PointLight.h>

class Scene {
private:
	std::vector<m<Object0>> Objects;
	std::vector<m<Camera>> Cameras;

	std::vector<m<DirectionalLight>> DirectionalLights;
	std::vector<m<PointLight>> PointLights;

	Light0::LightBuffer directLightBuffer, pointLightBuffer;

	void clear();
	void syncLights();
	void drawFrame(Camera &cam);

public:
	Scene();
	~Scene();

	void add(m<Object0> obj);
	void add(m<Camera> cam);
	void add(m<PointLight> light);
	void add(m<DirectionalLight> light);
	
	bool update();
	void resize();
	void pickCenterPixel(glm::vec2 sz);

	enum BufferBinding {
		DIRECTIONAL_LIGHT = 0,
		POINT_LIGHTS = 1,
		CAMERA_VIEW = 2,
		INSTANCED_TRANSFORM = 3,
	};
};

#endif