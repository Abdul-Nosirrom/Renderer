#pragma once 

#ifdef BUILD_DLL
	#define RENDERENGINE_API __declspec(dllexport)
#else 
	#define RENDERENGINE_API __declspec(dllimport)
#endif 