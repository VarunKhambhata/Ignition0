/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <glm/gtx/euler_angles.hpp>

#include <Ignition0Core/Object0.h>

void 		 Object0::add(m<Object0> obj) 					{ child.push_back(obj); 								}
void 		 Object0::setProjection(glm::mat4 projection) 	{ Projection = projection;	STATE|=PROJECTION_CHANGED;	}
void 		 Object0::setMaterial(m<Material0> mat) 		{ material = mat?mat:internal::Ignition0.missing; 		}
m<Material0> Object0::getMaterial()  						{ return material;   									}
glm::mat4&   Object0::getProjection() 						{ return Projection; 									}
glm::mat4&   Object0::getTransformation() 					{ return Transformation; 								}
glm::vec3 	 Object0::getPosition() 						{ return Position; 		 								}
glm::vec3 	 Object0::getRotation() 						{ return Rotation; 		 								}

Object0::Object0(): Projection(1), Transformation(1), Translation(1), Orientation(1), Position(0), Rotation(0), STATE(0) {}

void Object0::draw() {
	onDraw();	
	for(m<Object0> c: child) c->draw();
}

void Object0::update() {
	for(m<Script0> s: script) s->update();

	if(STATE) {
		applyStateUpdate();
		STATE = 0;
	}

	for(m<Object0> c: child) c->update(Projection);
}

void Object0::update(glm::mat4 projection) {
	setProjection(projection);
	update();
}

void Object0::normalizeRotation() {
	Rotation.x = ((Rotation.x = fmod(Rotation.x + 180, 360)) < 0 ? Rotation.x + 360 : Rotation.x) - 180;
	Rotation.y = ((Rotation.y = fmod(Rotation.y + 180, 360)) < 0 ? Rotation.y + 360 : Rotation.y) - 180;
	Rotation.z = ((Rotation.z = fmod(Rotation.z + 180, 360)) < 0 ? Rotation.z + 360 : Rotation.z) - 180;
}

void Object0::applyStateUpdate() {
	if(STATE & POSITION_CHANGED) {
		glm::vec3 Pos = Position;
		Pos.z = -Pos.z;
		Translation = glm::translate(glm::mat4(1), Pos);
	}

	if(STATE & ROTATION_CHANGED) {
		float pitch = glm::radians(Rotation.x);
		float yaw   = glm::radians(Rotation.y);
		float roll  = glm::radians(Rotation.z);
		Orientation = glm::eulerAngleYXZ(-yaw, -pitch, roll);
	}	

	Transformation = Translation * Orientation;

	if(STATE & PROJECTION_CHANGED) {
		Projection *= Transformation;
	}
}

void Object0::addScript(m<Script0> script)
{
	if(script->bindTo(this)) {
		script->start();
		this->script.push_back(script);
	}
}

void Object0::setPosition(float x, float y, float z) {	
	Position.x = x;
	Position.y = y;
	Position.z = z;

	STATE |= POSITION_CHANGED;
}

void Object0::translate(float x, float y, float z) {
	Position.x += x;
	Position.y += y;
	Position.z += z;

	STATE |= POSITION_CHANGED;
}

void Object0::rotate(float x, float y, float z) {
	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
	normalizeRotation();

	STATE |= ROTATION_CHANGED;
}