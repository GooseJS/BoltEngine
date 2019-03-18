#pragma once

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"
#include "BoltEngine/logger/Logger.h"
#include "BoltEngine/window/Window.h"

namespace Bolt
{
	class BOLT_API BoltApplication
	{
	private:
		Window _mainWindow;
		// TODO (Brendan): Have an array of child windows?
	protected:
		virtual void onStartup() {}

		virtual void draw(float updateDistance) {}
		virtual void update() {}

		virtual void onShutdown() {}

		inline Window& getMainWindow() { return _mainWindow; }

	public:
		void main();
	};
}