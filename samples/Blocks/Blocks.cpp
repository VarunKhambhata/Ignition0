/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <Ignition0.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Plane.h>
#include <Ignition0Core/Cube.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/Object0.h>
#include <Ignition0Core/UnlitImage.h>

#include "CamController.h"
#include "CubeAnimate.h"

m<Scene> buildScene();

int main() {
	Ignition0.start("Ignition0 ~ Blocks", 1400, 800);
	Ignition0.ignite(buildScene());
	return 0;
}

m<Scene> buildScene() {
	m<UnlitImage> wood = make<UnlitImage>();
	m<UnlitImage> brick = make<UnlitImage>();
	m<UnlitImage> controls = make<UnlitImage>();

	wood->setTexture("data/wood.jpg");
	brick->setTexture("data/brick.jpg");
	controls->setTexture("data/controls.jpg");

	m<Camera> cam = make<Camera>();	
	cam->addScript(make<CamControler>());
	
	m<Scene> scene = make<Scene>();
	scene->add(cam);

	m<Plane> UI = make<Plane>();
	UI->setPosition(3.2,-1.4,8);
	UI->rotate(30,45,5);
	UI->setMaterial(controls);
	cam->add(UI);

	for(int i=0; i< 5; i++) {
		for(int j=0; j < 5; j++) {
			for(int k=0; k<5; k++) {
				m<Cube> cube = make<Cube>();
				cube->setPosition(i * 8 - 15, j * 8 - 15, k * 8 - 15);
				cube->setMaterial(k*i % 2 ? wood : brick);
				cube->addScript(make<CubeAnimate>());
				scene->add(cube);
			}
		}		
	}

	return scene;
}