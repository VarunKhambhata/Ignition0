/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __OBJECT0__
#define __OBJECT0__

#include <memory>
#include <vector>
#include <functional>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/Script0.h>

class Object0 {
private:
	glm::mat4 Projection, Transformation, Translation, Orientation;
	std::vector<m<Object0>> child;
	std::vector<m<Script0>> script;

	void draw();
	void update();
	void update(glm::mat4 projection);

protected:
	glm::vec3 Position, Rotation;
	m<Material0> material = internal::Ignition0.missing;

	uint8_t STATE = 0;
	enum {
		POSITION_CHANGED   = 1,
		ROTATION_CHANGED   = 2,
		PROJECTION_CHANGED = 4,
	};

	void 		 normalizeRotation();
	glm::mat4&   getProjection();
	void 		 setProjection(glm::mat4 projection);
	virtual void applyStateUpdate();

public:
	Object0();

	void 		 add(m<Object0> obj);
	void 		 addScript(m<Script0> script);
	void         setMaterial(m<Material0> mat);

	m<Material0>  getMaterial();
	glm::vec3    getPosition();
	glm::vec3    getRotation();
	glm::mat4&   getTransformation();

	virtual void onDraw() {};
	virtual void setPosition(float x, float y, float z);
	virtual void translate(float x, float y, float z);
	virtual void rotate(float x, float y, float z);


	friend class Scene;
};

#endif