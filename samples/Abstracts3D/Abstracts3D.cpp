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

#include "UnlitNegative.h"

m<Scene> buildScene();

int main() {
	Ignition0.start("Ignition0 ~ Abstracts3D", 800, 800);
	Ignition0.ignite(buildScene());
	return 0;
}


void rotateCam(Object0& obj) {
	obj.rotate(0,0,0.3);
}

void rotateCube(Object0& obj) {
	obj.rotate(0.1,0.05,0.12);
}


m<Scene> buildScene() {
	m<UnlitColor>    red      = make<UnlitColor>(1,0,0);
	m<UnlitColor> 	 white    = make<UnlitColor>(1,1,1);
	m<UnlitColor> 	 blue     = make<UnlitColor>(0,0,1);
	m<UnlitColor> 	 violet   = make<UnlitColor>(0.5,0,0.3);
	m<UnlitColor> 	 green    = make<UnlitColor>(0.2,0.5,0.1);
	m<UnlitColor>	 orange	  = make<UnlitColor>(1,0.6,0.2);
	m<UnlitNegative> negative = make<UnlitNegative>();


	m<Plane> p1 = make<Plane>();
	p1->setMaterial(red);
	p1->setPosition(1 ,-1, 5);

		m<Plane> p4 = make<Plane>();
		p4->setMaterial(white);
		p4->translate(2,2,8);
		p4->rotate(0,50,0);
		p1->add(p4);

	m<Plane> p2 = make<Plane>();
	p2->setMaterial(blue);
	p2->setPosition(.1 ,0, 3);
	p2->rotate(0,10,0);

	m<Plane> p3 = make<Plane>();
	p3->setMaterial(violet);
	p3->translate(-2.2,-2, 10);
	p3->rotate(-30,-40, 90);

	m<Plane> p5 = make<Plane>();
	p5->setMaterial(green);
	p5->translate(-2,1,-8);
	p5->rotate(0,50,20);

	m<Cube> c1 = make<Cube>();
	c1->setMaterial(orange);
	c1->setPosition(-1.5,2.8,12);
	c1->setOnUpdate(rotateCube);

	m<Camera> cam1 = make<Camera>(0.025, 0.025, 0.4, 0.4);
	cam1->projection(30, 0.1f, 100.0f);
	cam1->setPosition(0.0f, 0.0f, -3.0f);		
	cam1->setBackground(0.2f, 0.3f, 0.3f, 1.0f);
	cam1->setMaterial(negative);

	m<Camera> cam2 = make<Camera>();
	cam2->projection(45, 0.1f, 100.0f);
	cam2->setPosition(0.9f, 0.2f, -5.0f);
	cam2->setBackground(0.35f, 0.35f, 0.35f, 1.0f);
	cam2->setOnUpdate(rotateCam);
	cam2->add(p5);

	m<Scene> scene = make<Scene>();
	scene->add(cam1);
	scene->add(cam2);
	scene->add(p2);
	scene->add(c1);
	scene->add(p1);
	scene->add(p3);
		

	return scene;
}
