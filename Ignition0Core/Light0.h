/**
 * Author:	Varun Khambhata
 * Created: 22.11.2024
**/

#ifndef __LIGHT0__
#define __LIGHT0__

#include <Ignition0Core/Object0.h>

class Light0: public Object0 {
private:
	struct LightBuffer {
	private:
		GLuint bufferObject;
		int size;
		int head;
		bool needsUpdate;

	public:
		LightBuffer();
		unsigned int readHead();
		void resetHead();
		bool next();

		friend class Scene;
	};

	void updateLight(LightBuffer& lBuffer);

protected:
	glm::vec3 Color;
	float Intensity;

public:
	Light0();
	void setColor(float r, float g, float b);
	void setIntensity(float intensity);
	virtual void updateUBO(const int index) = 0;

	friend class Scene;
};

#endif