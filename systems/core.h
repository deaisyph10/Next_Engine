#pragma once
#include <memory>
#include <functional>
#include <event/event.h>

#ifdef EX_PLATFORM_WINDOWS
#if EX_DYNAMIC_LINK
#ifdef EX_BUILD_DLL	
#define EXODUS_API __declspec(dllexport)
#else
#define EXODUS_API __declspec(dllimport)
#endif
#else
#define EXODUS_API
#endif
#else
#endif

#ifdef EX_ENABLE_ASSERTS
#define EX_ASSERTS(x, ...) { if(!(x)) { EX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define EX_CORE_ASSERT(x, ...) { if(!(x)) { EX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define EX_ASSERTS(x, ...)
#define EX_CORE_ASSERT(x, ...)
#endif

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

// In one of the headers, add an include guard:
#ifndef BIT
#define BIT(x) (1 << x)
#endif


#define EX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Exodus {


	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

}

