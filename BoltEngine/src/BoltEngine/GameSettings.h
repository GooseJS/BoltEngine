#pragma once

#include <chrono>

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"

#define BOLT_GET_CURRENT_TIME_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define BOLT_GET_CURRENT_TIME_SECONDS std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace Bolt
{
	class BOLT_API BoltTime
	{
	private:
		double _deltaTime = 0;
		double _lastFrameTime = 0;
		double _currentTime =  0;
	public:
		BoltTime() { _currentTime = glfwGetTime() * 1000; _deltaTime = _currentTime; _lastFrameTime = _currentTime; }

		double getLastFrameTime() const { return _lastFrameTime; }
		double getCurrentTime() const { return _currentTime; }
		float getDeltaTime() const { return _deltaTime; }

		inline void newFrame()
		{
			_currentTime = glfwGetTime() * 1000;
			_deltaTime = _currentTime - _lastFrameTime;
			_lastFrameTime = _currentTime;
		}
	};

	class BOLT_API InternalData
	{
	public:
		float oldMouseX = 0;
		float oldMouseY = 0;
	};

	class BOLT_API GameSettings
	{
	private:
		GameSettings() {}
		~GameSettings() = default;
		GameSettings(const GameSettings& gs) {}
		GameSettings& operator=(const GameSettings& gs) = delete;

		friend class BoltApplication;
		BoltTime _gameTime;
		InternalData _internalData;
		Window* _mainWindow;
	public:
		static GameSettings& getInstance()
		{
			static GameSettings instance;
			return instance;
		}

		BoltTime& getGameTime()
		{
			return _gameTime;
		}

		InternalData& getInternalData()
		{
			return _internalData;
		}

		Window& getMainWindow()
		{
			return *_mainWindow;
		}
	};
}