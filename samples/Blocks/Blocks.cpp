/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <Ignition0.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Plane.h>
#include <Ignition0Core/Cube.h>
#include <Ignition0Core/Sphere.h>
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
	Ignition0.setDetail(Detail::HIGH);
	Ignition0.ignite(buildScene());
	return 0;
}

m<Scene> buildScene() {
	m<LitImage> wood = make<LitImage>();
	m<LitImage> brick = make<LitImage>();
	m<UnlitImage> controls = make<UnlitImage>();
	m<LitColor> metalGreen = make<LitColor>(0.2, 1, 0.2,  0.0, 0.2, 5, 256);
	m<LitColor> metalWhite = make<LitColor>(1, 1, 1,  0.4, 0.8, 8, 256);

	wood->setTexture("data/wood.jpg");			wood->setMaterialProperties(0.5, 0.8, 1.2, 128);
	brick->setTexture("data/brick.jpg");		brick->setMaterialProperties(0.3, 1.8, 0.2, 32);
	controls->setTexture("data/controls.jpg");


	m<Camera> cam = make<Camera>();
	cam->setPosition(0,0,-30);
	cam->setProjection(52, 0.1f, 400.0f);
	cam->setBackground(0.1f, 0.1f, 0.2f);
	cam->addScript(make<CamControler>());
	

	m<Plane> UI = make<Plane>();
	UI->setPosition(4,-2,8);
	UI->rotate(-30,45,-5);
	UI->setMaterial(controls);
	cam->add(UI);


	m<PointLight> p1 = make<PointLight>();
	p1->setPosition(-8, 15, -10);
	p1->setColor(0.4, 0.3, 0.2);
	p1->setIntensity(5);
	p1->setProperties(25, 5);
	p1->addScript(make<CubeAnimate>());

	m<PointLight> p2 = make<PointLight>();
	p2->setPosition(10, -8, 13);
	p2->setColor(0.1, 0.2, 0.4);
	p2->setIntensity(6);
	p2->setProperties(12, 0.5);
	p2->addScript(make<CubeAnimate>());

	m<PointLight> p3 = make<PointLight>();
	p3->setColor(0.5, 0.3, 0.0);
	p3->setIntensity(5);
	p3->setProperties(30, 2);
	p3->addScript(make<CubeAnimate>());


	m<Cube> lightBoxWarm1 = make<Cube>();
	lightBoxWarm1->setMaterial(make<UnlitColor>(1,1,0));
	lightBoxWarm1->setScale(0.5, 0.5, 0.5);

	m<Cube> lightBoxWarm2 = make<Cube>();
	lightBoxWarm2->setMaterial(make<UnlitColor>(1,1,0));
	lightBoxWarm2->setScale(0.5, 0.5, 0.5);

	m<Cube> lightBoxCool = make<Cube>();
	lightBoxCool->setMaterial(make<UnlitColor>(0.0, 0.7, 1.0));
	lightBoxCool->setScale(0.5, 0.5, 0.5);



	p1->add(lightBoxWarm1);
	p3->add(lightBoxWarm2);
	p2->add(lightBoxCool);


	m<Plane> plane = make<Plane>();
	plane->setMaterial(metalWhite);
	plane->setPosition(0, -20, 0);
	plane->setRotation(90,0,0);
	plane->setScale(20, 20, 1);
	

	m<Scene> scene = make<Scene>();
	scene->add(p1);
	scene->add(p2);
	scene->add(p3);
	scene->add(cam);
	scene->add(plane);

	m<Sphere> sph1 = make<Sphere>();
	sph1->setMaterial(metalWhite);
	sph1->setPosition(-24, 10, 24);
	sph1->setScale(4,4,4);

	m<Sphere> sph2 = make<Sphere>();
	sph2->setMaterial(metalWhite);
	sph2->setPosition(-24, 10, -20);
	sph2->setScale(4,4,4);

	m<Sphere> sph3 = make<Sphere>();
	sph3->setMaterial(metalWhite);
	sph3->setPosition(26, 10, 24);
	sph3->setScale(4,4,4);

	m<Sphere> sph4 = make<Sphere>();
	sph4->setMaterial(metalWhite);
	sph4->setPosition(26, 10, -20);
	sph4->setScale(4,4,4);


	scene->add(sph1);
	scene->add(sph2);
	scene->add(sph3);
	scene->add(sph4);

	for(int i=0; i< 5; i++) {
		for(int j=0; j < 5; j++) {
			for(int k=0; k<5; k++) {
				m<Cube> cube = make<Cube>();
				cube->setPosition(i * 8 - 15, j * 8 - 15, k * 8 - 15);

				m<Material0> material;
				if(k*i % 2)      material = wood;
				else if(k*j % 3) material = metalGreen;
				else             material = brick;

				cube->setMaterial(material);
				cube->addScript(make<CubeAnimate>());
				scene->add(cube);
			}
		}
	}

	return scene;
}