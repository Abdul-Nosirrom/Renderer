#pragma once

#ifdef BUILD_DLL
    #define GEO_API __declspec(dllexport)
#else
    #define GEO_API __declspec(dllimport)
#endif

#define PI 3.14159265358979323846f
#define DEG_TO_RAD(x) x * PI / 180.f
#define RAD_TO_DEG(x) x * 180.f / PI