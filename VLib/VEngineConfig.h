#pragma once

#ifdef V_EXPORT
	#define V_API __declspec(dllexport)
#else
	#define V_API __declspec(dllimport)
#endif
