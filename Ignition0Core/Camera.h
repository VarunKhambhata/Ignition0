/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __CAMERA__
#define __CAMERA__

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

	glm::vec3 Front, Up;

	void updateTransformation();

public:
	Camera(float x = 0, float y = 0, float width = 1, float height = 1);
	~Camera();

	void open();
	void reload();

	void setPosition(float x, float y, float z) override;
	void translate(float x, float y, float z) override;
	void rotate(float x, float y, float z) override;

	void lookAt(glm::vec3 target);

	void projection(float FOV, float near, float far);
	void setBackground(float r, float g, float b, float a = 1);

	glm::vec2  getViewPosition();
	glm::vec2  getViewSize();
	glm::ivec2 getDisplayPosition();
	glm::ivec2 getDisplaySize();

	GLuint getRenderedTexture();
	GLuint getRenderedDepth();
};

#endif
