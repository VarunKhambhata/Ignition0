/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Camera.h>

GLuint 	   Camera::getRenderedTexture()	{ return colorBuffer; }
GLuint 	   Camera::getRenderedDepth()   { return depthBuffer; }
glm::vec2  Camera::getViewPosition()    { return vPosition;	  }
glm::vec2  Camera::getViewSize() 	    { return vSize;		  }
glm::ivec2 Camera::getDisplayPosition() { return dPosition;	  }
glm::ivec2 Camera::getDisplaySize()     { return dSize;		  }
glm::mat4& Camera::getProjection()		{ return Projection;  }

Camera::Camera(float x, float y, float width, float height): background(0,0,0,1), Front(0,0,1), Up(0,1,0), Right(1,0,0), Transformation(1) {
	glm::vec2 display = InternalIgnition0::displaySize();
	vPosition.x = x; 				vSize.x = width;
	vPosition.y = y;				vSize.y = height;
	dPosition.x = x * display.x;	dSize.x = width  * display.x;
	dPosition.y = y * display.y;	dSize.y = height * display.y;

	material = InternalIgnition0::colorImage;
	visible = false;

	// init uniformm buffer
	glGenBuffers(1, &cameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2 + sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);	

	// init framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// init and attach colorbuffer
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dSize.x, dSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

	//init and attach depthbuffer
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dSize.x, dSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);	
}

Camera::~Camera() {	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glDeleteTextures(1, &colorBuffer);
	glDeleteTextures(1, &depthBuffer);
	glDeleteBuffers(1, &cameraUBO);
}

void Camera::open() {
	glBindBufferBase(GL_UNIFORM_BUFFER, Scene::BufferBinding::CAMERA_VIEW, cameraUBO);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, dSize.x, dSize.y);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(background.r, background.g, background.b, background.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::close() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(dPosition.x, dPosition.y, dSize.x, dSize.y);
}

void Camera::reload() {
	glm::vec2 display = InternalIgnition0::displaySize();
	dPosition.x = vPosition.x * display.x;
	dPosition.y = vPosition.y * display.y;
	dSize.x 	= vSize.x 	  * display.x;
	dSize.y 	= vSize.y 	  * display.y;

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dSize.x, dSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dSize.x, dSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	setProjection(FOV, nearPlane, farPlane);
}

void Camera::lookAt(float x, float y, float z) {
	Front = glm::normalize(glm::vec3(x,y,z) - Position);
	Right = glm::normalize(glm::cross(glm::vec3(0,1,0), Front));
	Up    = glm::rotate(glm::normalize(glm::cross(Front, Right)), glm::radians(Rotation.z), Front);

	Rotation.x = glm::degrees(asin(-Front.y));
	Rotation.y = glm::degrees(atan2(Front.x,Front.z));

	if(Rotation.x >  89) Rotation.x = 89;
	if(Rotation.x < -89) Rotation.x = -89;
	normalizeRotation();

	PENDING_STATE |= ROTATION_CHANGED;
}

void Camera::setProjection(float FOV, float nearPlane, float farPlane) {
	this->FOV = FOV;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	Projection = glm::perspectiveLH(glm::radians(FOV), (float)dSize.x/dSize.y, nearPlane, farPlane);

	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &Projection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::setBackground(float r, float g, float b, float a) {
	background.r = r;
	background.g = g;
	background.b = b;
	background.a = a;
}

void Camera::rotate(float x, float y, float z) {
	if(!(x || y || z)) return;

	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;

	if(Rotation.x >  89) Rotation.x = 89;
	if(Rotation.x < -89) Rotation.x = -89;
	normalizeRotation();

	Front.z = cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
    Front.y = sin(glm::radians(-Rotation.x));
    Front.x = sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
    Front   = glm::normalize(Front);
    Right   = glm::normalize(glm::cross(glm::vec3(0,1,0),Front));
    Up      = glm::rotate(glm::normalize(glm::cross(Front, Right)), glm::radians(-Rotation.z), Front);

	PENDING_STATE |= ROTATION_CHANGED;
}

void Camera::translate(float x, float y, float z) {
	Position += x * Right;
	Position += y * Up;
	Position += z * Front;

	PENDING_STATE |= POSITION_CHANGED;
}

void Camera::applyStateUpdate() {
	if(PENDING_STATE)
		Transformation = glm::lookAt(Position, Position - Front, Up);
	glm::mat4 view = Transformation * glm::inverse(getGlobalTransformation());

	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, sizeof(glm::vec3), &Position);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Rotation.x = -Rotation.x;
	Object0::applyStateUpdate();
	Rotation.x = -Rotation.x;
}