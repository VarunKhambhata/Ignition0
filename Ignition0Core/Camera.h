/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __CAMERA0__
#define __CAMERA0__

#include <Ignition0Core/Object0.h>

class Camera: public Object0 {
private:
	glm::vec2  vPosition, vSize;
	glm::ivec2 dPosition, dSize;
	glm::vec4  background;

	GLuint frameBuffer;
	GLuint colorBuffer;
	GLuint depthBuffer;

	float FOV, near, far;

	glm::vec3 Front, Right, Up;

protected:
	void applyStateUpdate() override final;	

public:
	Camera(float x = 0, float y = 0, float width = 1, float height = 1);
	~Camera();

	GLuint 	   getRenderedTexture();
	GLuint 	   getRenderedDepth();
	glm::vec2  getViewPosition();
	glm::vec2  getViewSize();
	glm::ivec2 getDisplayPosition();
	glm::ivec2 getDisplaySize();

	void open();
	void reload();

	void lookAt(float x, float y, float z);
	void projection(float FOV, float near, float far);
	void setBackground(float r, float g, float b, float a = 1);

	void rotate(float x, float y, float z) override;
	void translate(float x, float y, float z) override;
};

#endif