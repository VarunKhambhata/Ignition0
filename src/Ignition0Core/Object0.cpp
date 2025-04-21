/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/Object0.h>

void 		 Object0::add(m<Object0> obj) 			{ child.push_back(obj);	PENDING_STATE |= 16; }
void 		 Object0::setMaterial(m<Material0> mat) { material = mat; 							 }
m<Material0> Object0::getMaterial()  				{ return material;   						 }
glm::mat4&	 Object0::getGlobalTransformation() 	{ return TransfromationGlobal; 				 }
glm::vec3 	 Object0::getPosition() 				{ return Position; 		 					 }
glm::vec3 	 Object0::getRotation() 				{ return Rotation; 		 					 }
glm::vec3 	 Object0::getScale()	 				{ return Scale; 		 					 }
void Object0::setVisible(bool visibility)			{ visible = visibility;						 }
bool Object0::isVisible()							{ return visible; 							 }
bool Object0::onExtension()							{ return false;								 }
bool Object0::drawAllowed = false;

Object0::Object0(): visible(true), childVisible(true), TransfromationGlobal(1), Orientation(1),
					Position(0), Rotation(0), Scale(1), PENDING_STATE(POSITION_CHANGED|ROTATION_CHANGED), STATE(0),
					material(InternalIgnition0::missing) 
					{}

void Object0::onDraw(RenderInfo& rInfo) {}

void Object0::draw(RenderInfo& rInfo) {
	if(visible) {
		onDraw(rInfo);
		if(childVisible) {
			for(m<Object0> c: child)
				c->draw(rInfo);
		}
	}
}

void Object0::requestDraw(Object0& object, RenderInfo& rInfo) {
	if(drawAllowed)
		object.draw(rInfo);
}

uint8_t Object0::update(const glm::mat4& parentTransform, uint8_t parentState) {
	for(m<Script0> s: script) 
		s->update();

	STATE = PENDING_STATE;

	if(PENDING_STATE | parentState) {
		TransfromationGlobal = parentTransform;
		applyStateUpdate();
		PENDING_STATE = 0;
	}

	uint8_t CHILD_STATE = 0;
	for(m<Object0> c: child) {
		CHILD_STATE |= c->update(prepChildUpdateTransformation(), STATE | parentState);
	}
	
	return STATE | CHILD_STATE;
}

const glm::mat4& Object0::prepChildUpdateTransformation() {
	return TransfromationGlobal;
}

void Object0::normalizeRotation() {
	Rotation.x = ((Rotation.x = fmod(Rotation.x + 180, 360)) < 0 ? Rotation.x + 360 : Rotation.x) - 180;
	Rotation.y = ((Rotation.y = fmod(Rotation.y + 180, 360)) < 0 ? Rotation.y + 360 : Rotation.y) - 180;
	Rotation.z = ((Rotation.z = fmod(Rotation.z + 180, 360)) < 0 ? Rotation.z + 360 : Rotation.z) - 180;
}

void Object0::applyStateUpdate() {
	if(PENDING_STATE & (ROTATION_CHANGED | SCALE_CHANGED)) {
		float pitch = glm::radians(Rotation.x);
		float yaw   = glm::radians(Rotation.y);
		float roll  = glm::radians(Rotation.z);
		Orientation = glm::eulerAngleYXZ(yaw, -pitch, -roll) * glm::scale(Scale);
	}
	TransfromationGlobal *= glm::translate(Position) * Orientation;
}

void Object0::add(m<Script0> script) {
	if(script->bindTo(this)) {
		script->start();
		this->script.push_back(script);
	}
}

void Object0::setPosition(float x, float y, float z) {
	Position.x = x;
	Position.y = y;
	Position.z = z;

	PENDING_STATE |= POSITION_CHANGED;
}

void Object0::translate(float x, float y, float z) {
	Position.x += x;
	Position.y += y;
	Position.z += z;

	PENDING_STATE |= POSITION_CHANGED;
}


void Object0::setRotation(float x, float y, float z) {
	Rotation.x = x;
	Rotation.y = y;
	Rotation.z = z;
	normalizeRotation();

	PENDING_STATE |= ROTATION_CHANGED;
}

void Object0::rotate(float x, float y, float z) {
	if(!(x || y || z)) return;

	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
	normalizeRotation();

	PENDING_STATE |= ROTATION_CHANGED;
}

void Object0::setScale(float x, float y, float z) {
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;

	PENDING_STATE |= SCALE_CHANGED;
}

void Object0::childList(std::vector<m<Object0>>::iterator& begin, std::vector<m<Object0>>::iterator& end) {
	begin = child.begin();
	end = child.end();
}

void Object0::extend(void callback(const void*), const void *obj) {
	if(onExtension()) {
		callback(obj);
	}

	for(m<Object0> c: child)
		c->extend(callback, obj);
}