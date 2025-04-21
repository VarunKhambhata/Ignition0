/**
 * Author:	Varun Khambhata
 * Created: 12.08.2024
**/

#include <Ignition0.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/Plane.h>
#include <Ignition0Core/Sphere.h>
#include <Ignition0Core/Line.h>
#include <Ignition0Core/Model.h>
#include <Ignition0Core/LitColor.h>
#include <Ignition0Core/LitImage.h>
#include <Ignition0Core/PointLight.h>
#include <Ignition0Core/DirectionalLight.h>
#include <Ignition0Core/MassRender.h>

#include "CamController.h"
#include "TopViewCam.h"

m<Scene> loadScene();

int main() {
	Ignition0::launch("Ignition0 ~ Models", 1600, 900);
	Ignition0::setDetail(Detail::LOW);
	Ignition0::ignite(loadScene());
	return 0;
}

m<Scene> loadScene() {	
	m<Camera> cam1 = make<Camera>();
	cam1->setProjection(60, 0.1f, 500.0f);
	cam1->setBackground(0.05, 0.02, 0.0);
	cam1->setPosition(0,3,-25);
	cam1->add(make<CamControler>());

	m<Camera> cam2 = make<Camera>(0.78, 0.78, 0.2,0.2);
	cam2->setProjection(20, 0.1f, 500.0f);
	cam2->setBackground(0,0,0);
	cam2->setPosition(0,80,-25);
	cam2->rotate(180,0,0);
	cam2->add(make<TopViewCam>(cam1));


	m<PointLight> pLT = make<PointLight>();
	pLT->setPosition(0, 65, 0);
	pLT->setColor(0.7, 0.7, 0.7);
	pLT->setIntensity(2);
	pLT->setProperties(240, 2);

	m<Sphere> lightSph = make<Sphere>();
	lightSph->setMaterial(make<UnlitColor>(1.0, 1.0, 0.7));
	lightSph->setScale(2,2,2);

	m<Line> l1 = make<Line>();
	l1->setMaterial(lightSph->getMaterial());
	l1->setScale(10,0,5);
	l1->translate(-5,0,0);

	m<Line> l2 = make<Line>();
	l2->setMaterial(lightSph->getMaterial());
	l2->setScale(10,0,5);
	l2->translate(0,5,0);
	l2->rotate(0,0,90);

	pLT->add(lightSph);
	pLT->add(l1);
	pLT->add(l2);

	m<DirectionalLight> dirLT = make<DirectionalLight>();
    dirLT->setColor(1.0, 0.3, 0.0);
    dirLT->setIntensity(0.2);
    dirLT->setRotation(45,-30,0);


	m<LitColor> dGreen = make<LitColor>();  dGreen->setMaterialProperties(0.1, 0.8, 0.0,  0.1, 0.6, 0.0, 0);
	m<LitColor> lGreen = make<LitColor>();  lGreen->setMaterialProperties(0.1, 0.8, 0.1,  0.4, 1.8, 0.2, 64);
	m<LitImage> stone = make<LitImage>(); stone->loadImage("textures/stone.jpg"); stone->setMaterialProperties(0.4, 1.8, 0.2, 64);
	m<LitImage> mud   = make<LitImage>(); mud->loadImage("textures/ground.jpg");  mud->setMaterialProperties(0.0, 0.1, 0.2, 0);	

	m<Plane> ground = make<Plane>();
	ground->rotate(90,0,0);
	ground->setScale(10, 10, 1);
	ground->setMaterial(mud);

	m<Model> tree = make<Model>();
	tree->load("models/LowpolyTree/Tree_low.obj");
	tree->setMaterial(dGreen);
	tree->translate(0,0.1,0);
	tree->setScale(0.2, 0.2, 0.2);

	m<Model> rock = make<Model>();
	rock->load("models/LowpolyRock/Rock.obj");
	rock->setMaterial(stone);
	rock->translate(0,0.45, 0);

	m<Model> grass = make<Model>();
	grass->load("models/LowpolyGrass/Grass.obj");
	grass->setMaterial(lGreen);
	grass->setPosition(-0.5,0.45, -0.5);
	grass->setScale(2,2,2);

	m<Model> grass2 = grass->clone();
	grass2->setScale(4,4,4);
	grass2->setPosition(0.5,0.45, -0.5);
	
	m<Model> grass3 = grass->clone();
	grass3->setScale(3,3,3);
	grass3->setPosition(0.5,0.45, 0.5);
	
	m<MassRender> groundMass = make<MassRender>();
	m<MassRender> detailsMass = make<MassRender>();
	m<MassRender> treeMass = make<MassRender>();	

	std::vector<MassRender::TransformVecs> groundTransfroms;
	groundTransfroms.reserve(400 * 400);
    for(int i = -200; i <= 200; i += 20 ) {
    	for(int j = -200; j <= 200; j += 20) {			
			groundTransfroms.push_back( {{i, 0, j}, {0,90,0}, {1,1,1}} );
    	}
    }

	std::vector<MassRender::TransformVecs> rockTransfroms;
	rockTransfroms.reserve(400 * 400);
    for(int i = -200; i <= 200; i += 5 ) {
    	for(int j = -200; j <= 200; j += 5) {
			rockTransfroms.push_back( {{i, 0.45, j}, {0, i*j*20, 0}, {1,1,1}} );
    	}
    }

	std::vector<MassRender::TransformVecs> treeTransfroms;
	treeTransfroms.reserve(725);
	treeTransfroms.push_back( {{0,0,0}, {0,0,0}, {1.5,1.5,1.5}});
	for (int r = 1, pointsPerRing = 15; r <= 16; r ++, pointsPerRing +=4) {
		double radius = r * (30-r);

		for (int p = 0; p < pointsPerRing; ++p) {
			double angle = (2 * M_PI / pointsPerRing) * p + (5 * r);
			double x = radius * std::cos(angle);
			double y = radius * std::sin(angle);
			float scale = 1.2 - r / 30.0f;

			treeTransfroms.push_back({{x,0.1,y}, {0, radius * 10 + x * y, 0}, {scale,scale,scale}});
		}
	}

	detailsMass->setMassTransforms(rockTransfroms);
	groundMass->setMassTransforms(groundTransfroms);
	treeMass->setMassTransforms(treeTransfroms);

	detailsMass->add(grass);
	detailsMass->add(grass2);
	detailsMass->add(grass3);
	detailsMass->add(rock);

	groundMass->add(ground);
	treeMass->add(tree);

	m<Scene> scene = make<Scene>();
	scene->add(cam2);
	scene->add(cam1);
	scene->add(groundMass);
	scene->add(detailsMass);
	scene->add(treeMass);
	scene->add(pLT);
	scene->add(dirLT);

	return scene;
}