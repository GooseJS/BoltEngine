#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"
#include "BoltEngine/InputEvents.h"
#include "BoltEngine/window/Window.h"
#include "BoltEngine/render/Shader.h"

namespace Bolt
{
	struct ImGuiViewportDataGlfw
	{
		GLFWwindow* Window;
		bool        WindowOwned;

		ImGuiViewportDataGlfw() { Window = NULL; WindowOwned = false; }
		~ImGuiViewportDataGlfw() { IM_ASSERT(Window == NULL); }
	};

	class BOLT_API BoltImGui
	{
	public:
		// OpenGL Data
		char			_glslVersionString[32] = "#version 330";
		GLuint			_fontTexture = 0;
		Shader			_shader{};
		int				_attribLocationTex = 0, _attribLocationProjMtx = 0;
		int				_attribLocationPosition = 0, _attribLocationUV = 0, _attribLocationColor = 0;
		unsigned int	_vboHandle = 0, _elementsHandle = 0;

		// GLFW data
		Window*			_window;
		bool			_mouseJustPressed[5] = { false, false, false, false, false };
		GLFWcursor*		_mouseCursors[ImGuiMouseCursor_COUNT] = { 0 };
		bool			_wantUpdateMonitors = true;

	private:
		void setup();

		bool createFontsTextures();
		void destroyFontsTextures();
		bool createDeviceObjects();
		void destroyDeviceObjects();
		void initPlatformInterface();
		void shutdownPlatformInterface();

		void updateMonitors();
		void updateMousePosAndButtons();
		void updateMouseCursor();
	public:
		static BoltImGui& getInstance()
		{
			static BoltImGui instance;
			return instance;
		}

		ImGuiContext* init(Window& window);
		void newFrame();
		void shutdown();
		void renderDrawData(ImDrawData* drawData);
		void render();

		void mouseButtonEvent(MouseButtonEvent event);
		void scrollEvent(EventMouseScroll event);
		void keyboardEvent(KeyboardKeyEvent event);
		// TODO: Character event
	};
}