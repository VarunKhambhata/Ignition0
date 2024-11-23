/**
 * Author:	Varun Khambhata
 * Created: 12.08.2024
**/

#include <Ignition0.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/Object0.h>
#include <Ignition0Core/Sphere.h>
#include <Ignition0Core/Line.h>
#include <Ignition0Core/Model.h>
#include <Ignition0Core/LitColor.h>
#include <Ignition0Core/LitImage.h>
#include <Ignition0Core/PointLight.h>
#include <Ignition0Core/DirectionalLight.h>

#include "../Blocks/CamController.h"

m<Scene> buildScene();

int main() {
	Ignition0.start("Ignition0 ~ Models", 1664, 836);
	Ignition0.setDetail(Detail::LOW);
	Ignition0.ignite(buildScene());
	return 0;
}

m<Scene> buildScene() {	
	m<Camera> cam = make<Camera>();	
	cam->setPosition(0,3,-25);
	cam->setProjection(60, 0.1f, 200.0f);
	cam->setBackground(0.05, 0.02, 0.0);
	cam->addScript(make<CamControler>());


	m<PointLight> pLT = make<PointLight>();
	pLT->setPosition(0, 30, 0);
	pLT->setColor(0.7, 0.7, 0.7);
	pLT->setIntensity(4);
	pLT->setProperties(60, 5);

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


	m<LitColor> green = make<LitColor>(0.1, 0.8, 0.0,  0.1, 0.6, 0.0, 0);
	m<LitImage> stone = make<LitImage>(); stone->setTexture("textures/stone.jpg"); stone->setMaterialProperties(0.4, 1.8, 0.2, 64);
	m<LitImage> mud   = make<LitImage>(); mud->setTexture("textures/ground.jpg");  mud->setMaterialProperties(0.0, 0.1, 0.2, 0);

	m<Plane> ground = make<Plane>();
	ground->rotate(90,0,0);
	ground->setScale(52, 52, 1);
	ground->setMaterial(mud);

	m<Model> tree = make<Model>();
	tree->load("models/LowpolyTree/Tree_low.obj");
	tree->setMaterial(green);
	tree->translate(0,0.1,0);
	tree->setScale(0.2, 0.2, 0.2);

	m<Model> rock = make<Model>();
	rock->load("models/LowpolyRock/Rock.obj");
	rock->setMaterial(stone);
	rock->translate(0,0.45, 0);
	


	m<Scene> scene = make<Scene>();
	scene->add(cam);
	scene->add(ground);
	scene->add(tree);
	scene->add(rock);
	scene->add(pLT);
	scene->add(dirLT);


	for (int r = 1, pointsPerRing = 8; r <= 5; r ++, pointsPerRing += 2) {
        double radius = r * (15-r);
        
        for (int p = 0; p < pointsPerRing; ++p) {
            double angle = (2 * M_PI / pointsPerRing) * p + (5 * r);
            double x = radius * std::cos(angle);
            double y = radius * std::sin(angle);
            float scale = 0.2 - r / 40.0f;

            m<Model> t = tree->clone();
            t->setPosition(x,0.1,y);
            t->setScale(scale, scale, scale);
            t->rotate(0, radius * 10 + x * y, 0);
            scene->add(t);
        }
    }

    for(int i = -45; i <= 45; i += 5 ) {
    	for(int j = -45; j <= 45; j += 5) {
    		m<Model> r = rock->clone();
    		r->setPosition(i, 0.45, j);
    		r->rotate(0, i*j*20, 0);
    		scene->add(r);
    	}
    }

	return scene;
}
