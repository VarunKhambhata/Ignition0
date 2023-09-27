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
	glm::mat4 Projection, Transformation, Translation, Orientation;
	std::vector<m<Object0>> child;
	std::function<void(Object0&)> onUpdate;

protected:
	glm::vec3 Position, Rotation;
	m<Material> material = internal::Ignition0.missing;

	uint8_t STATE = 0;
	enum {
		POSITION_CHANGED = 1,
		ROTATION_CHANGED = 2,
	};

	void normalizeRotation();
	virtual void applyStateUpdate();

public:
	Object0();

	void 		add(m<Object0> obj);
	void 		setOnUpdate(std::function<void(Object0&)> func);
	void 		setProjection(glm::mat4 projection);
	void        setMaterial(m<Material> mat);
	m<Material> getMaterial();
	glm::vec3   getPosition();
	glm::vec3   getRotation();
	glm::mat4&  getProjection();
	glm::mat4&  getTransformation();

	virtual void setPosition(float x, float y, float z);
	virtual void translate(float x, float y, float z);
	virtual void rotate(float x, float y, float z);

	void update();
	void update(glm::mat4 projection);
	virtual void onDraw() {};

};

#endif