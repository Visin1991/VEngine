#pragma once

constexpr auto engine_version = "v0.1";

#define API_GRAPHICS_D3D11

#define API_INPUT_WINDOWS

#ifdef V_RUNTIME_EXPORT
	#define V_API __declspec(dllexport)
#else
	#define V_API __declspec(dllimport)
#endif

// Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX 

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma warning(disable: 4506)

#include <assert.h>

namespace V
{
	template<typename T>
	constexpr void safe_delete(T*& ptr)
	{
		if (ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	template<typename T>
	constexpr void safe_release(T* ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

}