#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "BoltEngine/Core.h"

namespace Bolt
{
	class BOLT_API Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getEngineLogger() { return _engineLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return _clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> _engineLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
	};
}

#define BOLT_ENGINE_TRACE(...)		::Bolt::Logger::getEngineLogger()->trace(__VA_ARGS__)
#define BOLT_ENGINE_INFO(...)		::Bolt::Logger::getEngineLogger()->info(__VA_ARGS__)
#define BOLT_ENGINE_WARN(...)		::Bolt::Logger::getEngineLogger()->warn(__VA_ARGS__)
#define BOLT_ENGINE_ERROR(...)		::Bolt::Logger::getEngineLogger()->error(__VA_ARGS__)
#define BOLT_ENGINE_CRITICAL(...)	::Bolt::Logger::getEngineLogger()->critical(__VA_ARGS__)

#define BOLT_TRACE(...)				::Bolt::Logger::getClientLogger()->trace(__VA_ARGS__)
#define BOLT_INFO(...)				::Bolt::Logger::getClientLogger()->info(__VA_ARGS__)
#define BOLT_WARN(...)				::Bolt::Logger::getClientLogger()->warn(__VA_ARGS__)
#define BOLT_ERROR(...)				::Bolt::Logger::getClientLogger()->error(__VA_ARGS__)
#define BOLT_CRITICAL(...)			::Bolt::Logger::getClientLogger()->critical(__VA_ARGS__)