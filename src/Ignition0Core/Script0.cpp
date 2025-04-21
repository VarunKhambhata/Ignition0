/**
 * Author:	Varun Khambhata
 * Created: 02.10.2023
**/

#include <Ignition0Core/Object0.h>
#include <Ignition0Core/Script0.h>

Object0&  Script0::boundObject() { return *object0;				  }
glm::vec3 Script0::getPosition() { return object0->getPosition(); }
glm::vec3 Script0::getRotation() { return object0->getRotation(); }

Script0::Script0() : object0(nullptr) {}

bool Script0::bindTo(void* obj0) {
	if(!object0)
		object0 =  static_cast<Object0*>(obj0);
	return object0;
}

void Script0::setPosition(float x, float y, float z) {
	object0->setPosition(x,y,z);
}

void Script0::translate(float x, float y, float z) {
	object0->translate(x,y,z);
}

void Script0::setRotation(float x, float y, float z) {
	object0->setRotation(x,y,z);
}

void Script0::rotate(float x, float y, float z) {
	object0->rotate(x,y,z);
}

void Script0::setScale(float x, float y, float z) {
	object0->setScale(x,y,z);
}