#include "BoltEngine/BoltApplication.h"

#include "BoltEngine/GameSettings.h"

namespace Bolt
{
	void BoltApplication::main()
	{
		if (_mainWindow.createWindow() != SUCCESS)
		{
			return; // TODO (Brendan): Handle this?
		}
		GameSettings::getInstance()._mainWindow = &_mainWindow;

		BOLT_ENGINE_INFO("Starting application.");

		onStartup();

		double millisPerUpdate = 1 / 20.0f;

		double lag = 0.0f;
		BOLT_ENGINE_INFO("Starting main loop.");
		while (!_mainWindow.shouldClose())
		{
			GameSettings::getInstance().getGameTime().newFrame();
			lag += GameSettings::getInstance().getGameTime().getDeltaTime();

			_mainWindow.updateWindow();

			while (lag >= millisPerUpdate)
			{
				update();
				lag -= millisPerUpdate;
			}

			draw(lag / millisPerUpdate);
			_mainWindow.swapBuffers();
		}

		BOLT_ENGINE_INFO("Quitting!");

		onShutdown();
	}
}