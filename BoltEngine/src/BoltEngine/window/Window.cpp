#include "BoltEngine/window/Window.h"

#include "BoltEngine/InputEvents.h"
#include "BoltEngine/GameSettings.h"
#include "BoltEngine/logger/Logger.h"
#include "BoltEngine/event/EventSystem.h"
#include "BoltEngine/window/WindowEvents.h"

namespace Bolt
{
	Window::Window()
	{

	}

	Window::Window(const WindowCfg& windowCfg) : _windowCfg(windowCfg)
	{

	}

	Window::~Window()
	{
		if (_windowCreated)
			glfwDestroyWindow(_window);
	}

	BoltWindowCreationError Window::createWindow()
	{
		Bolt::Logger::init();

		if (glfwInit() != GLFW_TRUE)
		{
			// TODO (Brendan): More in depth logging and handling
			BOLT_ENGINE_CRITICAL("Could not initialize GLFW.");
			return GLFW_ERROR;
		}

		BOLT_ENGINE_INFO("Creating window...");
		_window = glfwCreateWindow(_windowCfg.windowWidth, _windowCfg.windowHeight, _windowCfg.windowTitle.c_str(), nullptr, nullptr);

		if (_window == nullptr)
		{
			// TODO (Brendan): More in depth logging and handling
			BOLT_ENGINE_CRITICAL("Could not create window.");
			return WINDOW_ERROR;
		}

		glfwMakeContextCurrent(_window);

		if (glewInit() != GLEW_OK)
		{
			// TODO (Brendan): More in depth logging and handling
			BOLT_ENGINE_CRITICAL("Could not initialize GLEW.");
			return GLEW_ERROR;
		}

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			EventWindowResize event(width, height);
			EventSystem::getInstance().dispatchEvent(event);
		});

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			EventWindowClose event;
			EventSystem::getInstance().dispatchEvent(event);
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			KeyboardKeyEvent globalEvent(key, scancode, mods, action);
			EventSystem::getInstance().dispatchEvent(globalEvent);
			switch (action)
			{
			case GLFW_PRESS:
			{
				EventKeyPressed event(key, scancode, mods);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				EventKeyReleased event(key, scancode, mods);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			case GLFW_REPEAT:
			{
				EventKeyRepeat event(key, scancode, mods);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			MouseButtonEvent globalEvent(button, xpos, ypos, action);
			EventSystem::getInstance().dispatchEvent(globalEvent);
			switch (action)
			{
			case GLFW_PRESS:
			{
				EventMouseButtonPressed event(button, xpos, ypos);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				EventMouseButtonPressed event(button, xpos, ypos);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			}
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			EventMouseMove event((float)xPos, (float)-yPos, (float)xPos - GameSettings::getInstance().getInternalData().oldMouseX, GameSettings::getInstance().getInternalData().oldMouseY - (float)yPos);
			EventSystem::getInstance().dispatchEvent(event);

			GameSettings::getInstance().getInternalData().oldMouseX = (float)xPos;
			GameSettings::getInstance().getInternalData().oldMouseY = (float)yPos;
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double x, double y)
		{
			EventMouseScroll event((float)x, (float)y);
			EventSystem::getInstance().dispatchEvent(event);
		});

		BOLT_ENGINE_INFO("Window created successfully.");
		_windowCreated = true;
		return SUCCESS;
	}
}