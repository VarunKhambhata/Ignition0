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
#include <Ignition0Core/LitColor.h>
#include <Ignition0Core/LitImage.h>
#include <Ignition0Core/PointLight.h>
#include "CamController.h"
#include "CubeAnimate.h"

m<Scene> buildScene();

int main() {
	Ignition0.start("Ignition0 ~ Blocks", 1280, 930);
	Ignition0.ignite(buildScene());
	return 0;
}

m<Scene> buildScene() {
	m<LitImage> wood = make<LitImage>();
	m<LitImage> brick = make<LitImage>();
	m<UnlitImage> controls = make<UnlitImage>();
	m<LitColor> metallic = make<LitColor>(0.2, 1, 0.2,  0.0, 0.2, 5, 256);

	wood->setTexture("data/wood.jpg");			wood->setMaterialProperties(0.5, 0.8, 1.2, 128);
	brick->setTexture("data/brick.jpg");		brick->setMaterialProperties(0.3, 1.8, 0.2, 32);
	controls->setTexture("data/controls.jpg");

	m<Camera> cam = make<Camera>();	
	cam->addScript(make<CamControler>());
	cam->setPosition(0,0,-10);
	
	m<Scene> scene = make<Scene>();
	scene->add(cam);

	m<Plane> UI = make<Plane>();
	UI->setPosition(4,-2,8);
	UI->rotate(30,45,5);
	UI->setMaterial(controls);
	cam->add(UI);



	m<PointLight> p1 = make<PointLight>();
	p1->setPosition(-8, 15, -10);
	p1->setColor(0.4, 0.3, 0.2);
	p1->setProperties(25, 5, 5);
	p1->addScript(make<CubeAnimate>());

	m<PointLight> p2 = make<PointLight>();
	p2->setPosition(10, -8, 13);
	p2->setColor(0.1, 0.2, 0.4);
	p2->setProperties(12, 6, 0.5);
	p2->addScript(make<CubeAnimate>());

	m<PointLight> p3 = make<PointLight>();
	p3->setColor(0.5, 0.3, 0.0);
	p3->setProperties(30, 5, 2);
	p3->addScript(make<CubeAnimate>());


	m<Cube> sharedLightBoxWarm = make<Cube>();
	sharedLightBoxWarm->setMaterial(make<UnlitColor>(1,1,0));

	m<Cube> lightBoxCool = make<Cube>();
	lightBoxCool->setMaterial(make<UnlitColor>(0.0, 0.7, 1.0));


	p1->add(sharedLightBoxWarm);
	p3->add(sharedLightBoxWarm);
	p2->add(lightBoxCool);
	
	scene->add(p1);
	scene->add(p2);
	scene->add(p3);


	for(int i=0; i< 5; i++) {
		for(int j=0; j < 5; j++) {
			for(int k=0; k<5; k++) {
				m<Cube> cube = make<Cube>();
				cube->setPosition(i * 8 - 15, j * 8 - 15, k * 8 - 15);

				m<Material0> material;
				if(k*i % 2)      material = wood;
				else if(k*j % 3) material = metallic;
				else             material = brick;

				cube->setMaterial(material);
				cube->addScript(make<CubeAnimate>());
				scene->add(cube);
			}
		}
	}

	return scene;
}
