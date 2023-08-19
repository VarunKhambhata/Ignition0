/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __Ignition0Engine__
#define __Ignition0Engine__


#include <GLFW/glfw3.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/InternalIgnition0.h>

namespace ______________ {
	namespace ________ {
		namespace _0{
				class __engine__ {
				private:
					static std::string title;
					static GLFWwindow* window;
					static m<Scene>    currentScene;

					static void initialize();
					static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
					static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
					static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
					static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
					static void renderLoop();

				public:
					static void start(std::string title, int width = 0, int height = 0);
					static void ignite(m<Scene> scene);
				};
			}
	}
}

static ______________::________::_0::__engine__
Ignition0;

#endif