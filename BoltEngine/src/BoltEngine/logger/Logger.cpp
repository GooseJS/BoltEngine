#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	std::shared_ptr<spdlog::logger> Logger::_engineLogger;
	std::shared_ptr<spdlog::logger> Logger::_clientLogger;

	void Logger::init()
	{
		spdlog::set_pattern("%^[%T] [%l] %n: %v%$");
		_engineLogger = spdlog::stdout_color_mt("BOLT");
		_engineLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_mt("APP"); // TODO (Brendan): Replace with app name??
		_clientLogger->set_level(spdlog::level::trace);
	}
}