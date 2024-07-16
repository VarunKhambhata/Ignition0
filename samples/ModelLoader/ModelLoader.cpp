/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <Ignition0.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/Object0.h>
#include <Ignition0Core/Model.h>
#include <Ignition0Core/LitColor.h>
#include <Ignition0Core/Sphere.h>
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
	cam->setBackground(0.0f, 0.0f, 0.05f);
	cam->addScript(make<CamControler>());


	m<PointLight> p1 = make<PointLight>();
	p1->setPosition(0, 30, 0);
	p1->setColor(0.7, 0.7, 0.7);
	p1->setProperties(200, 2.2, 5);

	m<Sphere> light = make<Sphere>();
	light->setMaterial(make<UnlitColor>(1.0, 1.0, 0.7));
	light->setScale(2,2,2);
	p1->add(light);



	m<LitColor> green = make<LitColor>(0.1, 0.8, 0.0,  0.1, 0.6, 0.0, 0);
	m<LitColor> white = make<LitColor>(0.5, 0.5, 0.5,  0.4, 2, 0.2, 64);
	m<LitColor> brown = make<LitColor>(0.9, 0.3, 0.0,  0.0, 0.1, 0.2, 0);

	m<Plane> ground = make<Plane>();
	ground->rotate(90,0,0);
	ground->setScale(52, 52, 1);
	ground->setMaterial(brown);

	m<Model> tree = make<Model>();
	tree->load("models/LowpolyTree/Tree_low.obj");
	tree->setMaterial(green);
	tree->translate(0,0.1,0);
	tree->setScale(0.2, 0.2, 0.2);

	m<Model> rock = make<Model>();
	rock->load("models/LowpolyRock/Rock.obj");
	rock->setMaterial(white);
	rock->translate(0,0.45, 0);


	m<Scene> scene = make<Scene>();
	scene->add(cam);
	scene->add(p1);
	scene->add(ground);
	scene->add(tree);
	scene->add(rock);


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