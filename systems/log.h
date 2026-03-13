#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#ifndef EXODUS_API
#define EXODUS_API
#endif
#include <spdlog/logger.h>
#include <memory>

namespace Exodus {

	class EXODUS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
//Core Log macros
#define EX_CORE_ERROR(...)    ::Exodus::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EX_CORE_INFO(...)     ::Exodus::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EX_CORE_TRACE(...)    ::Exodus::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EX_CORE_WARN(...)     ::Exodus::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EX_CORE_FATAL(...)    ::Exodus::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client Log macros
#define EX_ERROR(...)    ::Exodus::Log::GetClientLogger()->error(__VA_ARGS__)
#define EX_INFO(...)     ::Exodus::Log::GetClientLogger()->info(__VA_ARGS__)
#define EX_TRACE(...)    ::Exodus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EX_WARN(...)     ::Exodus::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EX_FATAL(...)    ::Exodus::Log::GetClientLogger()->fatal(__VA_ARGS__)
