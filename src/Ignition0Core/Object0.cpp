/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <Ignition0Core/Object0.h>

void Object0::normalizeRotation() {
	Rotation.x = ((Rotation.x = fmod(Rotation.x + 180, 360)) < 0 ? Rotation.x + 360 : Rotation.x) - 180;
	Rotation.y = ((Rotation.y = fmod(Rotation.y + 180, 360)) < 0 ? Rotation.y + 360 : Rotation.y) - 180;
	Rotation.z = ((Rotation.z = fmod(Rotation.z + 180, 360)) < 0 ? Rotation.z + 360 : Rotation.z) - 180;
}

void Object0::update(glm::mat4 projection) {
	setProjection(projection * Transformation);
	update();
}

void Object0::update() {
	onUpdate ? onUpdate(*this) : (void)0;
	onDraw();
	for(m<Object0> c: child) c->update(Projection);
}

void Object0::add(m<Object0> obj) { 
	child.push_back(obj); 
}

void Object0::setOnUpdate(std::function<void(Object0&)> func) {
	onUpdate = func;
}

void Object0::setMaterial(m<Material> mat) { 
	material = mat?mat:internal::Ignition0.missing;
}

m<Material> Object0::getMaterial() {
	return material;
}

glm::mat4& Object0::getTransformation() { return Transformation; }
glm::mat4& Object0::getProjection() 	{ return Projection;     }

void Object0::setProjection(glm::mat4 projection) { Projection = projection;}

glm::vec3 Object0::getPosition() { return Position; }
glm::vec3 Object0::getRotation() { return Rotation; }

void Object0::setPosition(float x, float y, float z) {
	Position.x = x;
	Position.y = y;
	Position.z = -z;
	Transformation = glm::translate(glm::mat4(1), Position);
	Position.z = z;
}

void Object0::translate(float x, float y, float z) {
	Position.x += x;
	Position.y += y;
	Position.z += z;
	Transformation = glm::translate(Transformation, glm::vec3(x,y,-z));
}

void Object0::rotate(float x, float y, float z) {
	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
	normalizeRotation();

	if(x) Transformation = glm::rotate(Transformation, glm::radians(x), glm::vec3(-1,  0, 0));
	if(y) Transformation = glm::rotate(Transformation, glm::radians(y), glm::vec3( 0, -1, 0));
	if(z) Transformation = glm::rotate(Transformation, glm::radians(z), glm::vec3( 0,  0, 1));
}