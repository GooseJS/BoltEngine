#pragma once

#include <string>

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"

namespace Bolt
{
	struct WindowCfg
	{
		int windowWidth = 1280;
		int windowHeight = 720;

		std::string windowTitle = "Bolt Engine";
	};

	enum BoltWindowCreationError
	{
		SUCCESS = 0,
		GLFW_ERROR = 1,
		WINDOW_ERROR = 2,
		GLEW_ERROR = 3
	};

	class BOLT_API Window
	{
	private:
		GLFWwindow* _window = nullptr;

		WindowCfg _windowCfg{};
		bool _windowCreated = false;

	public:
		Window();
		Window(const WindowCfg& windowCfg);
		~Window();

		BoltWindowCreationError createWindow();

		inline void updateWindow() { glfwPollEvents(); }
		inline void swapBuffers() { glfwSwapBuffers(_window); }

		inline bool shouldClose() { return glfwWindowShouldClose(_window) == GLFW_TRUE; }
		inline void setShouldClose() { glfwSetWindowShouldClose(_window, true); }

		inline bool isKeyPressed(int keyCode) { return glfwGetKey(_window, keyCode) != GLFW_RELEASE; }
		inline bool isMouseButtonPressed(int button) { return glfwGetMouseButton(_window, button) != GLFW_RELEASE; }

		inline double getMouseX() { double x = 0, y = 0; glfwGetCursorPos(_window, &x, &y); return x; }
		inline double getMouseY() { double x = 0, y = 0; glfwGetCursorPos(_window, &x, &y); return y; } // Note (Brendan): What the fuck

		inline GLFWwindow* getGLFWWindow() { return _window; } // TODO (Brendan): TBH, I don't really think this should be exposed. I think ImGui needs it but we can just make it a friend??
	};
}