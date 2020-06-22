#pragma once



#ifdef _WIN32
	#ifdef V_EXPORT
		#define V_API __declspec(dllexport)
	#else
		#define V_API __declspec(dllimport)
	#endif
#elif defined(__GNUC__)
	#ifdef V_EXPORT
		#define V_API __attribute__((visibility("default")))
	#else
		#define V_API 
	#endif
#else
	#define V_API
#endif
