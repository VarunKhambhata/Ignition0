/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __OBJECT0__
#define __OBJECT0__

#include <vector>

#include <Ignition0Core/Ignition0Environment.h>
#include <Ignition0Core/RenderInfo.h>
#include <Ignition0Core/Material0.h>
#include <Ignition0Core/Script0.h>

class Object0 {
private:
	glm::mat4 TransfromationGlobal, Orientation;
	
	std::vector<m<Object0>> child;
	std::vector<m<Script0>> script;

	static bool drawAllowed;

	void 	draw(RenderInfo& rInfo);
	uint8_t update(const glm::mat4& parentTransform, uint8_t parentState = 0);	

protected:
	bool visible, childVisible;
	glm::vec3 Position, Rotation, Scale;
	m<Material0> material;

	uint8_t PENDING_STATE, STATE;
	enum {
		POSITION_CHANGED = 1,
		ROTATION_CHANGED = 2,
		SCALE_CHANGED	 = 4,
		PROPERTY_CHANGED = 8,
		CHILD_ADDED      = 16,
	};

	void 		  normalizeRotation();
	glm::mat4&    getGlobalTransformation();
	void 		  childList(std::vector<m<Object0>>::iterator& begin, std::vector<m<Object0>>::iterator& end);

	virtual void  applyStateUpdate();
	virtual bool  onExtension();
	virtual void  onDraw(RenderInfo& rInfo);
	static  void  requestDraw(Object0& object, RenderInfo& rInfo);
	virtual const 
	glm::mat4& 	  prepChildUpdateTransformation();

public:
	Object0();

	void add(m<Object0> obj);
	void add(m<Script0> script);
	void setMaterial(m<Material0> mat);
	void setVisible(bool visibility);
	void extend(void callback(const void*), const void *obj);

	bool  		 isVisible();
	m<Material0> getMaterial();
	glm::vec3    getPosition();
	glm::vec3    getRotation();
	glm::vec3    getScale();

	virtual void setPosition(float x, float y, float z);
	virtual void translate(float x, float y, float z);
	virtual void setRotation(float x, float y, float z);
	virtual void rotate(float x, float y, float z);
	virtual void setScale(float x, float y, float z);

	friend class Scene;
};

#endif