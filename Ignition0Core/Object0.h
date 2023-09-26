/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __OBJECT0__
#define __OBJECT0__

#include <memory>
#include <vector>
#include <functional>

#include <Ignition0Core/InternalIgnition0.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Object0 {
private:
	glm::mat4 Projection 	 = glm::mat4(1.0f);
	glm::mat4 Transformation = glm::mat4(1.0f);
	glm::mat4 Translation    = glm::mat4(1.0f);
	glm::mat4 Orientation    = glm::mat4(1.0f);
	std::vector<m<Object0>> child;
	std::function<void(Object0&)> onUpdate;

protected:
	glm::vec3 Position, Rotation;
	m<Material> material = internal::Ignition0.missing;

	void normalizeRotation();

public:
	virtual void onDraw() {};
	void update();
	void update(glm::mat4 projection);
	void add(m<Object0> obj);
	void setOnUpdate(std::function<void(Object0&)> func);

	void        setMaterial(m<Material> mat);
	m<Material> getMaterial();
	glm::vec3   getPosition();
	glm::vec3   getRotation();
	glm::mat4&  getTransformation();
	glm::mat4&  getProjection();

	virtual void setPosition(float x, float y, float z);
	virtual void translate(float x, float y, float z);
	virtual void rotate(float x, float y, float z);

	void setProjection(glm::mat4 projection);
};

#endif
