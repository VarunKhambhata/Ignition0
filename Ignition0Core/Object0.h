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
#include <Ignition0Core/RenderView.h>
#include <Ignition0Core/Script0.h>

class Object0 {
private:
	glm::mat4 TransfromationGlobal, Orientation;

	std::vector<m<Object0>> child;
	std::vector<m<Script0>> script;

	void draw(const RenderView& rView);
	uint8_t update(const glm::mat4& parentTransform, uint8_t parentState = 0);

protected:
	bool visible;
	glm::vec3 Position, Rotation, Scale;
	m<Material0> material = internal::Ignition0.missing;

	uint8_t PENDING_STATE, STATE;
	enum {
		POSITION_CHANGED = 1,
		ROTATION_CHANGED = 2,
		SCALE_CHANGED	 = 3,
	};

	enum AttribLocation {
		VERTEX  = 0,
		TEXTURE = 1,
		NORMAL  = 2,
	};

	void 		 normalizeRotation();
	glm::mat4&   getGlobalTransformation();

	virtual void applyStateUpdate();
	virtual const glm::mat4& prepChildUpdateTransformation();

public:
	Object0();

	void add(m<Object0> obj);
	void addScript(m<Script0> script);
	void setMaterial(m<Material0> mat);
	void setVisible(bool visibility);

	bool  		 isVisible();
	m<Material0> getMaterial();
	glm::vec3    getPosition();
	glm::vec3    getRotation();
	glm::vec3    getScale();

	virtual void onDraw(const RenderView& rView);
	virtual void setPosition(float x, float y, float z);
	virtual void translate(float x, float y, float z);
	virtual void setRotation(float x, float y, float z);
	virtual void rotate(float x, float y, float z);
	virtual void setScale(float x, float y, float z);


	friend class Scene;
};

#endif
