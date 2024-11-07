#pragma once


#ifdef IGE_PLATFORM_WINDOWS
	#ifdef IGE_BUILD_DLL
		#define IGENGINE_API __declspec(dllexport)
	#else
		#define IGENGINE_API __declspec(dllimport)
	#endif
#else
#error IG Engine only supports Windows!
#endif