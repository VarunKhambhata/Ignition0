/**
 * Author:	Varun Khambhata
 * Created: 02.10.2023
**/

#ifndef __SCRIPT0__
#define __SCRIPT0__

class Script0 {
private:
	bool bindTo(void* obj0);
	class Object0 *object0;

protected:
	virtual void start() = 0;
	virtual void update() = 0;

	Object0* boundObject();

	glm::vec3 getPosition();
	glm::vec3 getRotation();

	void setPosition(float x, float y, float z);
	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);

	friend class Object0;
};

#endif