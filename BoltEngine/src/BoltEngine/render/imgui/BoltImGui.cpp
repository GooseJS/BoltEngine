#include "BoltEngine/render/imgui/BoltImGui.h"

#include "BoltEngine/GameSettings.h"
#include "BoltEngine/event/EventSystem.h"

#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>   // for glfwGetWin32Window
#endif

namespace Bolt
{
	static void BoltImGui_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(BoltImGui::getInstance()._mouseJustPressed))
			BoltImGui::getInstance()._mouseJustPressed[button] = true;
	}

	static void BoltImGui_scrollCallback(GLFWwindow* window, double x, double y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float)x;
		io.MouseWheel += (float)y;
	}

	static void BoltImGui_keyboardCallback(GLFWwindow* window, int key, int scancode, int mods, int action)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLFW_PRESS)
			io.KeysDown[key] = true;
		if (action == GLFW_RELEASE)
			io.KeysDown[key] = false;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	}

	static const char* BoltImGui_getClipboardText(void* user_data)
	{
		return glfwGetClipboardString((GLFWwindow*)user_data);
	}

	static void BoltImGui_setClipboardText(void* user_data, const char* text)
	{
		glfwSetClipboardString((GLFWwindow*)user_data, text);
	}

	static void BoltImGui_windowCloseCallback(GLFWwindow* window)
	{
		if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
			viewport->PlatformRequestClose = true;
	}

	static void BoltImGui_windowPosCallback(GLFWwindow* window, int, int)
	{
		if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
			viewport->PlatformRequestMove = true;
	}

	static void BoltImGui_windowSizeCallback(GLFWwindow* window, int, int)
	{
		if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
			viewport->PlatformRequestResize = true;
	}

	static void BoltImGui_createWindow(ImGuiViewport* viewport)
	{
		ImGuiViewportDataGlfw* data = IM_NEW(ImGuiViewportDataGlfw)();
		viewport->PlatformUserData = data;

		glfwWindowHint(GLFW_VISIBLE, false);
		glfwWindowHint(GLFW_FOCUSED, false);
		glfwWindowHint(GLFW_DECORATED, (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? false : true);
		GLFWwindow* shareWindow = GameSettings::getInstance().getMainWindow().getGLFWWindow();
		data->Window = glfwCreateWindow((int)viewport->Size.x, (int)viewport->Size.y, "No title yet", nullptr, shareWindow);
		data->WindowOwned = true;
		viewport->PlatformHandle = (void*)data->Window;
		glfwSetWindowPos(data->Window, (int)viewport->Pos.x, (int)viewport->Pos.y);

		glfwSetMouseButtonCallback(data->Window, BoltImGui_mouseButtonCallback);
		glfwSetScrollCallback(data->Window, BoltImGui_scrollCallback);
		glfwSetKeyCallback(data->Window, BoltImGui_keyboardCallback);
		// TODO: Character callback
		glfwSetWindowCloseCallback(data->Window, BoltImGui_windowCloseCallback);
		glfwSetWindowPosCallback(data->Window, BoltImGui_windowPosCallback);
		glfwSetWindowSizeCallback(data->Window, BoltImGui_windowSizeCallback);
		glfwMakeContextCurrent(data->Window);
		glfwSwapInterval(0);
	}

	static void BoltImGui_destroyWindow(ImGuiViewport* viewport)
	{
		if (ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData)
		{
			if (data->WindowOwned)
			{
				glfwDestroyWindow(data->Window);
			}
			data->Window = nullptr;
			IM_DELETE(data);
		}
		viewport->PlatformUserData = viewport->PlatformHandle = nullptr;
	}

	static void BoltImGui_showWindow(ImGuiViewport* viewport)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		// GLFW hack: Hide icon from task bar
		HWND hwnd = glfwGetWin32Window(data->Window);
		if (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
		{
			LONG ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
			ex_style &= ~WS_EX_APPWINDOW;
			ex_style |= WS_EX_TOOLWINDOW;
			::SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);
		}

		// GLFW hack: GLFW 3.2 has a bug where glfwShowWindow() also activates/focus the window.
		// The fix was pushed to GLFW repository on 2018/01/09 and should be included in GLFW 3.3 via a GLFW_FOCUS_ON_SHOW window attribute.
		if (viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
		{
			::ShowWindow(hwnd, SW_SHOWNA);
			return;
		}

		glfwShowWindow(data->Window);
	}

	static ImVec2 BoltImGui_getWindowPos(ImGuiViewport* viewport)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		int x = 0, y = 0;
		glfwGetWindowPos(data->Window, &x, &y);
		return ImVec2((float)x, (float)y);
	}

	static void BoltImGui_setWindowPos(ImGuiViewport* viewport, ImVec2 pos)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		glfwSetWindowPos(data->Window, (int)pos.x, (int)pos.y);
	}

	static ImVec2 BoltImGui_getWindowSize(ImGuiViewport* viewport)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		int w = 0, h = 0;
		glfwGetWindowSize(data->Window, &w, &h);
		return ImVec2((float)w, (float)h);
	}

	static void BoltImGui_setWindowSize(ImGuiViewport* viewport, ImVec2 size)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		glfwSetWindowSize(data->Window, (int)size.x, (int)size.y);
	}

	static void BoltImGui_setWindowTitle(ImGuiViewport* viewport, const char* title)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		glfwSetWindowTitle(data->Window, title);
	}

	static bool BoltImGui_getWindowFocus(ImGuiViewport* viewport)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		return glfwGetWindowAttrib(data->Window, GLFW_FOCUSED) != 0;
	}

	static bool BoltImGui_getWindowMinimized(ImGuiViewport* viewport)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		return glfwGetWindowAttrib(data->Window, GLFW_ICONIFIED) != 0;
	}

	static void BoltImGui_renderWindow(ImGuiViewport* viewport, void*)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		glfwMakeContextCurrent(data->Window);
	}

	static void BoltImGui_swapBuffers(ImGuiViewport* viewport, void*)
	{
		ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
		glfwSwapBuffers(data->Window);
	}

	static void BoltImGui_monitorCallback(GLFWmonitor*, int)
	{
		BoltImGui::getInstance()._wantUpdateMonitors = true;
	}

	static void BoltImGui_rendererRenderWindow(ImGuiViewport* viewport, void*)
	{
		if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear))
		{
			ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		BoltImGui::getInstance().renderDrawData(viewport->DrawData);
	}

	void BoltImGui::setup()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
		io.BackendRendererName = "imgui_apollo";

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		io.SetClipboardTextFn = BoltImGui_setClipboardText;
		io.GetClipboardTextFn = BoltImGui_getClipboardText;
		io.ClipboardUserData = _window->getGLFWWindow();

		ImGui::StyleColorsDark(); // TODO: Theming??

		_mouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		_mouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		_mouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
		_mouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		_mouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		_mouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		_mouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		_mouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

		ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		mainViewport->PlatformHandle = (void*)_window->getGLFWWindow();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			initPlatformInterface();
	}

	bool BoltImGui::createFontsTextures()
	{
		// Creating texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture to graphics card
		GLint lastTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGenTextures(1, &_fontTexture);
		glBindTexture(GL_TEXTURE_2D, _fontTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Storing the textureID;
		io.Fonts->TexID = (ImTextureID)(intptr_t)_fontTexture;

		// Restoring state
		glBindTexture(GL_TEXTURE_2D, lastTexture);

		return true; // TODO: Maybe some error checking?
	}

	void BoltImGui::destroyFontsTextures()
	{
		if (_fontTexture)
		{
			ImGuiIO& io = ImGui::GetIO();
			glDeleteTextures(1, &_fontTexture);
			io.Fonts->TexID = 0;
			_fontTexture = 0;
		}
	}

	bool BoltImGui::createDeviceObjects()
	{
		// Backup GL state
		GLint lastTexture, lastArrayBuffer, lastVertexArray;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);

		// Load Shader
		const GLchar* vertexShaderSource =
			"#version 130\n"
			"uniform mat4 ProjMtx;\n"
			"in vec2 Position;\n"
			"in vec2 UV;\n"
			"in vec4 Color;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		const GLchar* fragmentShaderSource =
			"#version 130\n"
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
			"}\n";

		_shader.initFromString(vertexShaderSource, fragmentShaderSource);

		_attribLocationTex = _shader.uniform("Texture");
		_attribLocationProjMtx = _shader.uniform("ProjMtx");
		_attribLocationPosition = _shader.attribute("Position");
		_attribLocationUV = _shader.attribute("UV");
		_attribLocationColor = _shader.attribute("Color");

		// Create Buffers
		glGenBuffers(1, &_vboHandle);
		glGenBuffers(1, &_elementsHandle);

		createFontsTextures();

		// Restore GL State
		glBindTexture(GL_TEXTURE_2D, lastTexture);
		glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
		glBindVertexArray(lastVertexArray);

		return true;
	}

	void BoltImGui::destroyDeviceObjects()
	{
		if (_vboHandle) _vboHandle = _elementsHandle = 0;

		destroyFontsTextures();
	}

	void BoltImGui::initPlatformInterface()
	{
		ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		platformIO.Platform_CreateWindow = BoltImGui_createWindow;
		platformIO.Platform_DestroyWindow = BoltImGui_destroyWindow;
		platformIO.Platform_ShowWindow = BoltImGui_showWindow;
		platformIO.Platform_SetWindowPos = BoltImGui_setWindowPos;
		platformIO.Platform_GetWindowPos = BoltImGui_getWindowPos;
		platformIO.Platform_SetWindowSize = BoltImGui_setWindowSize;
		platformIO.Platform_GetWindowSize = BoltImGui_getWindowSize;
		platformIO.Platform_GetWindowFocus = BoltImGui_getWindowFocus;
		platformIO.Platform_GetWindowMinimized = BoltImGui_getWindowMinimized;
		platformIO.Platform_SetWindowTitle = BoltImGui_setWindowTitle;
		platformIO.Platform_RenderWindow = BoltImGui_renderWindow;
		platformIO.Platform_SwapBuffers = BoltImGui_swapBuffers;
		platformIO.Renderer_RenderWindow = BoltImGui_rendererRenderWindow;

		glfwSetMonitorCallback(BoltImGui_monitorCallback);

		ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGuiViewportDataGlfw* data = IM_NEW(ImGuiViewportDataGlfw)();
		data->Window = _window->getGLFWWindow();
		data->WindowOwned = false;
		mainViewport->PlatformUserData = data;
		mainViewport->PlatformHandle = (void*)_window->getGLFWWindow();	
	}

	void BoltImGui::shutdownPlatformInterface()
	{

	}

	void BoltImGui::updateMonitors()
	{
		ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		int monitorCount = 0;
		GLFWmonitor** glfwMonitors = glfwGetMonitors(&monitorCount);
		platformIO.Monitors.resize(0);
		for (int n = 0; n < monitorCount; n++)
		{
			ImGuiPlatformMonitor monitor;
			int x, y;
			glfwGetMonitorPos(glfwMonitors[n], &x, &y);
			const GLFWvidmode* vidMode = glfwGetVideoMode(glfwMonitors[n]);
			monitor.MainPos = monitor.WorkPos = ImVec2((float)x, (float)y);
			monitor.MainSize = monitor.WorkSize = ImVec2((float)vidMode->width, (float)vidMode->height);
			platformIO.Monitors.push_back(monitor);
		}
	}

	void BoltImGui::updateMousePosAndButtons()
	{
		ImGuiIO& io = ImGui::GetIO();
		const ImVec2 mousePosBackup = io.MousePos;
		io.MousePos = ImVec2(0, 0);
		io.MouseHoveredViewport = 0;

		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
		{
			io.MouseDown[i] = _mouseJustPressed[i] || glfwGetMouseButton(_window->getGLFWWindow(), i) != 0;
			_mouseJustPressed[i] = false;
		}

		ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		for (int n = 0; n < platformIO.Viewports.Size; n++)
		{
			ImGuiViewport* viewport = platformIO.Viewports[n];
			GLFWwindow* window = (GLFWwindow*)viewport->PlatformHandle;
			IM_ASSERT(window != nullptr);
			const bool focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;

			if (focused)
			{
				if (io.WantSetMousePos)
				{
					glfwSetCursorPos(window, (double)(mousePosBackup.x - viewport->Pos.x), (double)(mousePosBackup.y - viewport->Pos.y));
				}
				else
				{
					double mouseX, mouseY;
					glfwGetCursorPos(window, &mouseX, &mouseY);
					if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
					{
						int windowX, windowY;
						glfwGetWindowPos(window, &windowX, &windowY);
						io.MousePos = ImVec2((float)mouseX + windowX, (float)mouseY + windowY);
					}
					else
					{
						io.MousePos = ImVec2((float)mouseX, (float)mouseY);
					}
				}
			}
			for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
				io.MouseDown[i] |= glfwGetMouseButton(window, i) != 0;
		}
	}

	void BoltImGui::updateMouseCursor()
	{
		ImGuiIO& io = ImGui::GetIO();
		if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(_window->getGLFWWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			return;

		ImGuiMouseCursor imguiCursor = ImGui::GetMouseCursor();
		ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		for (int n = 0; n < platformIO.Viewports.size(); n++)
		{
			GLFWwindow* window = (GLFWwindow*)platformIO.Viewports[n]->PlatformHandle;
			if (imguiCursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else
			{
				glfwSetCursor(window, _mouseCursors[imguiCursor] ? _mouseCursors[imguiCursor] : _mouseCursors[ImGuiMouseCursor_Arrow]);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	ImGuiContext* BoltImGui::init(Window& window)
	{
		_window = &window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		setup();

		EventSystem& es = EventSystem::getInstance();
		es.addListener<MouseButtonEvent>(BOLT_SUBSCRIBE_EVENT(&BoltImGui::mouseButtonEvent));
		es.addListener<KeyboardKeyEvent>(BOLT_SUBSCRIBE_EVENT(&BoltImGui::keyboardEvent));
		es.addListener<EventMouseScroll>(BOLT_SUBSCRIBE_EVENT(&BoltImGui::scrollEvent));

		createDeviceObjects();

		return ImGui::GetCurrentContext();
	}

	void BoltImGui::newFrame()
	{
		if (!_fontTexture)
			createDeviceObjects();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();

		int w, h;
		int displayWidth, displayHeight;
		glfwGetWindowSize(_window->getGLFWWindow(), &w, &h);
		glfwGetFramebufferSize(_window->getGLFWWindow(), &displayWidth, &displayHeight);
		io.DisplaySize = ImVec2((float)w, (float)h);
		io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)displayWidth / w) : 0, h > 0 ? ((float)displayHeight / h) : 0);

		if (_wantUpdateMonitors)
			updateMonitors();

		io.DeltaTime = GameSettings::getInstance().getGameTime().getDeltaTime();

		updateMousePosAndButtons();

		updateMouseCursor();

		ImGui::NewFrame();
	}

	void BoltImGui::shutdown()
	{
		for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
		{
			glfwDestroyCursor(_mouseCursors[cursor_n]);
			_mouseCursors[cursor_n] = nullptr;
		}
		destroyDeviceObjects();
	}

	void BoltImGui::renderDrawData(ImDrawData* drawData)
	{
		// Checking that there is something for us to render to, and also scaling for retina (using the frame buffer size, not screen size)
		int width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
		int height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
		if (width <= 0 || height <= 0)
			return;

		// Save the previous GL state
		GLint lastProgram; glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		GLenum lastActiveTexture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&lastActiveTexture);
		glActiveTexture(GL_TEXTURE0);
		GLint lastTexture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		GLint lastArrayBuffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
		GLint lastVertexArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);
		GLint lastViewport[4]; glGetIntegerv(GL_VIEWPORT, lastViewport);
		GLint lastScissorBox[4]; glGetIntegerv(GL_SCISSOR_BOX, lastScissorBox);
		GLenum lastBlendSrcRGB; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&lastBlendSrcRGB);
		GLenum lastBlendDstRGB; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&lastBlendDstRGB);
		GLenum lastBlendSrcAlpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&lastBlendSrcAlpha);
		GLenum lastBlendDstAlpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&lastBlendDstAlpha);
		GLenum lastBlendEquationRGB; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&lastBlendEquationRGB);
		GLenum lastBlendEquationAlpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&lastBlendEquationAlpha);
		GLboolean lastEnableBlend = glIsEnabled(GL_BLEND);
		GLboolean lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
		GLboolean lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
		GLboolean lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);

		// Setup render: enable alpha-blending, no face culling, no depth testing, and scissor enabled
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);

		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		float left = drawData->DisplayPos.x;
		float right = drawData->DisplayPos.x + drawData->DisplaySize.x;
		float top = drawData->DisplayPos.y;
		float bottom = drawData->DisplayPos.y + drawData->DisplaySize.y;

		glm::mat4 orthoMatrix = glm::ortho(left, right, bottom, top);

		_shader.use();
		glUniform1i(_attribLocationTex, 0);
		glUniformMatrix4fv(_attribLocationProjMtx, 1, GL_FALSE, &orthoMatrix[0][0]);

		// Recreate this VAO every time
		// (Apparently it is to easily allow multiple GL contexts, but IDK if i care about that???)
		GLuint vaoHandle = 0;
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);
		glBindBuffer(GL_ARRAY_BUFFER, _vboHandle);
		glEnableVertexAttribArray(_attribLocationPosition);
		glEnableVertexAttribArray(_attribLocationUV);
		glEnableVertexAttribArray(_attribLocationColor);
		glVertexAttribPointer(_attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(_attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(_attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

		// Will project scissor / clipping rectangles into framebuffer space
		ImVec2 clipOff = drawData->DisplayPos;			// (0, 0) unless using multi-viewports
		ImVec2 clipScale = drawData->FramebufferScale;	// (1, 1) unless using retina display which are often (2, 2)

		// Render command lists
		for (int n = 0; n < drawData->CmdListsCount; n++)
		{
			const ImDrawList* cmdList = drawData->CmdLists[n];
			size_t idxBufferOffset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, _vboHandle);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmdList->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmdList->VtxBuffer.Data, GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementsHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmdList->IdxBuffer.Data, GL_STREAM_DRAW);

			for (int cmd_i = 0; cmd_i < cmdList->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					// We have a user callback (registered vai ImDrawList::AddCallback)
					pcmd->UserCallback(cmdList, pcmd);
				}
				else
				{
					// Project scissor / clipping rectangle into framebuffer space
					ImVec4 clipRect;
					clipRect.x = (pcmd->ClipRect.x - clipOff.x) * clipScale.x;
					clipRect.y = (pcmd->ClipRect.y - clipOff.y) * clipScale.y;
					clipRect.z = (pcmd->ClipRect.z - clipOff.x) * clipScale.x;
					clipRect.w = (pcmd->ClipRect.w - clipOff.y) * clipScale.y;

					if (clipRect.x < width && clipRect.y < height && clipRect.z >= 0.0 && clipRect.w >= 0.0f)
					{
						// Apply scissor / clipping rectangle
						glScissor((int)clipRect.x, (int)(height - clipRect.w), (int)(clipRect.z - clipRect.x), (int)(clipRect.w - clipRect.y));

						// Bind the texture, and draw
						glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
						glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)idxBufferOffset);
					}
				}
				idxBufferOffset += pcmd->ElemCount * sizeof(ImDrawIdx);
			}
		}
		glDeleteVertexArrays(1, &vaoHandle);

		// Restore GL state
		glUseProgram(lastProgram);
		glBindTexture(GL_TEXTURE_2D, lastTexture);
		glActiveTexture(lastActiveTexture);
		glBindVertexArray(lastVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
		glBlendEquationSeparate(lastBlendEquationRGB, lastBlendEquationAlpha);
		glBlendFuncSeparate(lastBlendSrcRGB, lastBlendDstRGB, lastBlendSrcAlpha, lastBlendDstAlpha);
		if (lastEnableBlend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if (lastEnableCullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if (lastEnableDepthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if (lastEnableScissorTest) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
		glViewport(lastViewport[0], lastViewport[1], lastViewport[2], lastViewport[3]);
		glScissor(lastScissorBox[0], lastScissorBox[1], lastScissorBox[2], lastScissorBox[3]);
	}

	void BoltImGui::render()
	{
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();
		renderDrawData(drawData);
		ImGui::EndFrame();
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	// Event / GLFW callbacks
	void BoltImGui::mouseButtonEvent(MouseButtonEvent event)
	{
		BoltImGui_mouseButtonCallback(nullptr, event.button(), event.action(), 0);
	}

	void BoltImGui::scrollEvent(EventMouseScroll event)
	{
		BoltImGui_scrollCallback(nullptr, event.x(), event.y());
	}

	void BoltImGui::keyboardEvent(KeyboardKeyEvent event)
	{
		BoltImGui_keyboardCallback(nullptr, event.key(), event.scancode(), event.mods(), event.action());
	}
}