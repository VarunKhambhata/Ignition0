/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Ignition0.h>
#include <Ignition0Core/Plane.h>
#include <Ignition0Core/IgnitionInputs.h>
#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/Logger0.h>

namespace InternalIgnition0 {
	bool initialized;
	Detail detailLevel;
	glm::ivec2 displaysize;
	float deltatime;

	const glm::mat4 IDENTITY(1);
	const glm::vec3 ORIGIN(0);

	m<UnlitColor> missing;
	m<ColorImage2D> colorImage;
	m<Object0> plane;

	const unsigned int MAX_LIGHT = 1024;
	GLuint identityMat4SSBO;

	bool initialize() {
		bool ret = initialized;
		initialized = true;
		detailLevel = Detail::MEDIUM;
		deltatime = 0;
		return ret;
	}

	glm::ivec2 displaySize() {
		return displaysize; 
	}

	Detail preferedDetail() {
		return detailLevel;
	}

	float deltaTime() {
		return deltatime;
	}

	void enableInstanceMat4(GLuint &ssbo) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Scene::BufferBinding::INSTANCED_TRANSFORM, ssbo);
	}
}

inputs::Keyset Keys;
inputs::MouseDevice Mouse;
std::string title;
GLFWwindow* window;
m<Scene>    currentScene;
std::chrono::duration<double> vsyncInterval;

class EngineCore {
public:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		Logger0(title <<" Resized: " << width <<" " << height);
		InternalIgnition0::displaysize = {width, height};
		currentScene->resize();
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if(Keys.map[key])
			*Keys.map[key] = action;
	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		Mouse.pos(static_cast<int>(xpos), static_cast<int>(ypos));
	}
	
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		*Mouse.keyMap[button] = action;
	}

	static void initialize() {
		if(InternalIgnition0::initialize()) return;
			// GLFW & GLEW Init *********************************************************************************
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			vsyncInterval = std::chrono::duration<double>(1.0 / mode->refreshRate);

			void* monitorCreateWindow = nullptr;
			glm::ivec2 scrSize = InternalIgnition0::displaySize();
			if(scrSize.x == 0 || scrSize.y == 0) {
				scrSize.x = mode->width;
				scrSize.y = mode->height;
				InternalIgnition0::displaysize = {scrSize.x, scrSize.y};
				monitorCreateWindow = monitor;
			}
			window = glfwCreateWindow(scrSize.x, scrSize.y, title.c_str(), static_cast<GLFWmonitor*>(monitorCreateWindow), NULL);

			// glfwSetWindowPos(window, (mode->width - scrSize.x) / 2, (mode->height - scrSize.y) / 2);
			glfwMakeContextCurrent(window);
			glfwSetKeyCallback(window, key_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetCursorPosCallback(window, cursor_position_callback);
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glewInit();
			// **************************************************************************************************


			// Shared Assets Init *******************************************************************************
			InternalIgnition0::missing = make<UnlitColor>(1,0,1);
			InternalIgnition0::colorImage = make<ColorImage2D>();
			InternalIgnition0::plane = make<Plane>();

			glGenBuffers(1, &InternalIgnition0::identityMat4SSBO);
			InternalIgnition0::enableInstanceMat4(InternalIgnition0::identityMat4SSBO);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4), &InternalIgnition0::IDENTITY, GL_STATIC_DRAW);
			// **************************************************************************************************

			Logger0("Ignition0 Initialized");
	}

	static void renderLoop() {
		Logger0("Rendering Started");
		
		float lastFrame = 0.0f;
		while (!glfwWindowShouldClose(window)) {
			float currentFrame = glfwGetTime();
			InternalIgnition0::deltatime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			currentScene->update() ?
				glfwSwapBuffers(window) :
				std::this_thread::sleep_for(vsyncInterval);

			Mouse.clearDelta();
			glfwPollEvents();
		}
		Logger0("Rendering Finished");
	}

	static void clean() {
		glDeleteBuffers(1, &InternalIgnition0::identityMat4SSBO);
		glfwTerminate();
		Logger0("Ignition0 Stopped");
	}
};







void Ignition0::launch(std::string _title, int width, int height) {
	title = _title;
	InternalIgnition0::displaysize = {width, height};
	EngineCore::initialize();
}

void Ignition0::setDetail(Detail level) {
	InternalIgnition0::detailLevel = level;
}

void Ignition0::ignite(m<Scene> scene) {
	currentScene = scene;
	EngineCore::renderLoop();
	EngineCore::clean();
}