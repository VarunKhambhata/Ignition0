/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#define GLEW_STATIC
#define i0 __engine__

#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>

#include <Ignition0.h>
#include <Ignition0Core/IgnitionInputs.h>
#include <Ignition0Core/InternalIgnition0.h>
#include <Ignition0Core/Logger0.h>

namespace ______________ {
	namespace ________ {
		namespace _0{

			std::string i0::title;
			GLFWwindow* i0::window;
			m<Scene>   i0::currentScene;

			void i0::initialize() {
				if(internal::Ignition0.initialize()) return;

				// GLFW & GLEW Init *********************************************************************************
			    glfwInit();
			    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			   

			    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			    glm::ivec2 scrSize = internal::Ignition0.displaySize();

			    if(scrSize.x == 0 || scrSize.y == 0) {
			    	scrSize.x  = mode->width;
			    	scrSize.y = mode->height;
			    	internal::Ignition0.displaySize(scrSize.x, scrSize.y);
			    	window = glfwCreateWindow(scrSize.x, scrSize.y, title.c_str(), monitor, NULL);
			    }
			    else {
			    	window = glfwCreateWindow(scrSize.x, scrSize.y, title.c_str(), NULL, NULL);
			    }

			    // glfwSetWindowPos(window, (mode->width - scrSize.x) / 2, (mode->height - scrSize.y) / 2);
			    glfwMakeContextCurrent(window);			    
				glfwSetKeyCallback(window, key_callback);
				glfwSetMouseButtonCallback(window, mouse_button_callback);
				glfwSetCursorPosCallback(window, cursor_position_callback);
				glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
				glewInit();
				// **************************************************************************************************


				// Shared Assets Init *******************************************************************************
				internal::Ignition0.missing    = make<UnlitColor>(1,0,1);
				internal::Ignition0.colorImage = make<UnlitImage>();
				// **************************************************************************************************

				Logger0("Ignition0 Initialized");
			}

			void i0::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
				Logger0(title <<" Resized: " << width <<" " << height);
				internal::Ignition0.displaySize(width, height);
				currentScene->resize();
			}

			void i0::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
				if(Keys.map[key])
					*Keys.map[key] = action;
			}

			void i0::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {				
				Mouse.pos(static_cast<int>(xpos), static_cast<int>(ypos));
			}

			void i0::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
				*Mouse.keyMap[button] = action;
			}

			void i0::renderLoop() {
				Logger0("Rendering Started");
				while (!glfwWindowShouldClose(window)) {
			        currentScene->update();
					glfwSwapBuffers(window);

					Mouse.clearDelta();
			        glfwPollEvents();
				}
				Logger0("Rendering Finished");
			    glfwTerminate();
			    Logger0("Ignition0 Stopped");
			}

			void i0::start(std::string _title, int width, int height) {
				title = _title;
				internal::Ignition0.displaySize(width, height);
				initialize();
			}

			void i0::ignite(m<Scene> scene) {
				currentScene = scene;
				renderLoop();
			}

		}
	}
}



namespace internal {
	i0 Ignition0;

	bool i0::initialize() {
		bool ret = _initialized;
		_initialized = true;
		return ret;
	}

	void i0::displaySize(int w, int h) {
		_displaySize.x = w;
		_displaySize.y = h;
	}

	glm::ivec2 i0::displaySize() {
		return _displaySize; 
	}
}


inputs::Keyset Keys;
inputs::MouseDevice Mouse;
