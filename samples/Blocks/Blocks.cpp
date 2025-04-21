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
#include <Ignition0Core/LitColor.h>
#include <Ignition0Core/LitImage.h>
#include <Ignition0Core/UnlitColor.h>
#include <Ignition0Core/UnlitImage.h>
#include <Ignition0Core/PointLight.h>
#include <Ignition0Core/MassRender.h>

#include "CamController.h"
#include "CubeAnimate.h"
#include "AnimatePath.h"
#include "PostProcess.h"

m<Scene> loadScene();

int main() {
	Ignition0::launch("Ignition0 ~ Blocks", 1280, 930);
	Ignition0::setDetail(Detail::HIGH);
	Ignition0::ignite(loadScene());
	return 0;
}

m<Scene> loadScene() {
	m<LitImage> wood = make<LitImage>();
	m<LitImage> brick = make<LitImage>();
	m<UnlitImage> controls = make<UnlitImage>();
	m<LitColor> metalGreen = make<LitColor>();
	m<LitColor> metalWhite = make<LitColor>();
	m<UnlitColor> colorWarm = make<UnlitColor>(1,1,0);
	m<UnlitColor> colorCool = make<UnlitColor>(0.0, 0.7, 1.0);
	m<UnlitColor> colorWhite = make<UnlitColor>(1, 1, 1);
	m<UnlitColor> colorRed = make<UnlitColor>(1, 0.4, 0.1);

	wood->loadImage("data/wood.jpg");
	wood->setMaterialProperties(0.5, 0.8, 1.2, 128);
	brick->loadImage("data/brick.jpg");
	brick->setMaterialProperties(0.3, 1.8, 0.2, 32);
	metalGreen->setMaterialProperties(0.2, 1, 0.2,  0.0, 0.2, 5, 256);
	metalWhite->setMaterialProperties(1, 1, 1,  0.4, 0.8, 8, 256);
	controls->loadImage("data/controls.jpg");


	m<Camera> cam = make<Camera>();
	cam->setPosition(0,0,-30);
	cam->setProjection(52, 0.1f, 400.0f);
	cam->setBackground(0.1f, 0.1f, 0.2f);
	cam->add(make<CamControler>());
	cam->setMaterial(make<PostProcess>());

	m<Plane> UI = make<Plane>();
	UI->setPosition(4,-2,8);
	UI->rotate(-30,45,-5);
	UI->setMaterial(controls);
	cam->add(UI);

	
	m<Cube> lightBoxWarm1 = make<Cube>();
	m<Cube> lightBoxWarm2 = make<Cube>();
	m<Cube> lightBoxCool = make<Cube>();
	m<Cube> lightBoxWhite = make<Cube>();
	m<Cube> lightBoxRed = make<Cube>();

	lightBoxWhite->setScale(2.8, 2.8, 2.8);
	lightBoxWhite->setMaterial(colorWhite);

	lightBoxRed->setScale(2.8, 2.8, 2.8);
	lightBoxRed->setMaterial(colorRed);

	lightBoxWarm1->setScale(0.8, 0.8, 0.8);	
	lightBoxWarm1->setMaterial(colorWarm);
	
	lightBoxWarm2->setScale(0.8, 0.8, 0.8);
	lightBoxWarm2->setMaterial(colorWarm);

	lightBoxCool->setScale(0.8, 0.8, 0.8);
	lightBoxCool->setMaterial(colorCool);


	
	m<PointLight> p1 = make<PointLight>();
	m<PointLight> p2 = make<PointLight>();
	m<PointLight> p3 = make<PointLight>();
	m<PointLight> p4 = make<PointLight>();
	m<PointLight> p5 = make<PointLight>();

	p1->setPosition(-8, 15, -10);
	p1->setColor(0.4, 0.3, 0.2);
	p1->setIntensity(5);
	p1->setProperties(25, 5);
	p1->add(lightBoxWarm1);
	p1->add(make<CubeAnimate>());
	
	p2->setPosition(10, -8, 13);
	p2->setColor(0.1, 0.2, 0.4);
	p2->setIntensity(6);
	p2->setProperties(12, 0.5);
	p3->add(lightBoxWarm2);
	p2->add(make<CubeAnimate>());
	
	p3->setColor(0.5, 0.3, 0.0);
	p3->setIntensity(5);
	p3->setProperties(30, 2);
	p2->add(lightBoxCool);
	p3->add(make<CubeAnimate>());

	p4->setColor(0.5, 0.5, 0.5);
	p4->setIntensity(1);
	p4->setProperties(180, 2);
	p4->add(lightBoxWhite);
	p4->add(make<AnimatePath>(std::vector<glm::vec3>({ {-80, 0, -80}, {-80, 70, 0}, {-80, 0, 80}, {0, 70, 80}, {80, 0, 80}, {80, 70, 0}, {80, 0, -80}, {0, 70, -80} })));

	p5->setColor(0.8, 0.3, 0.1);
	p5->setIntensity(5);
	p5->setProperties(120, 1);
	p5->add(lightBoxRed);
	p5->add(make<AnimatePath>(std::vector<glm::vec3>({ {80, 70, 80}, {80, 220, 0}, {80, 70, -80}, {0, 220, -80}, {-80, 70, -80}, {-80, 220, 0}, {-80, 70, 80}, {0, 220, 80} })));


	m<Plane> base = make<Plane>();
	base->setMaterial(metalWhite);
	base->setPosition(0, -20, 0);
	base->setRotation(90,0,0);
	base->setScale(20, 20, 1);


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
	

	m<Object0> objGroup = make<Object0>();
	objGroup->add(base);
	objGroup->add(sph1);
	objGroup->add(sph2);
	objGroup->add(sph3);
	objGroup->add(sph4);

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
				cube->add(make<CubeAnimate>());
				objGroup->add(cube);
			}
		}
	}

	m<MassRender> worldClones = make<MassRender>();
	worldClones->add(objGroup);
	worldClones->setMassTransforms({
		{glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(1)},
		{glm::vec3(65,10,0), glm::vec3(0,0,0), glm::vec3(1.3)},
		{glm::vec3(-65,10,0), glm::vec3(0,0,0), glm::vec3(1.3)},
		{glm::vec3(0,10,65), glm::vec3(0,0,0), glm::vec3(1.3)},
		{glm::vec3(0,10,-65), glm::vec3(0,0,0), glm::vec3(1.3)},
		{glm::vec3(65,0,65), glm::vec3(0,45,0), glm::vec3(1)},
		{glm::vec3(65,0,-65), glm::vec3(0,45,0), glm::vec3(1)},
		{glm::vec3(-65,0,65), glm::vec3(0,45,0), glm::vec3(1)},
		{glm::vec3(-65,0,-65), glm::vec3(0,45,0), glm::vec3(1)},
		{glm::vec3(0,200,0), glm::vec3(180,0,0), glm::vec3(5,5,5)},
	});


	m<Scene> scene = make<Scene>();
	scene->add(p1);
	scene->add(p2);
	scene->add(p3);
	scene->add(p4);
	scene->add(p5);
	scene->add(cam);
	scene->add(worldClones);
	return scene;
}