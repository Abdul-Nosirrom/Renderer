#pragma once

#include <vector>
#include <d3d11.h>
#include <string>

struct VertexFactory;

class GeometryFactory
{
public:
	enum class EType : UINT
	{
		Cube, Plane, Sphere, Cylinder
	};

	static std::string MakeCube(VertexFactory& vertices);
	static std::string MakePlane(VertexFactory& vertices, UINT tesselation = 2);
	static std::string MakeSphere(VertexFactory& vertices, UINT phiSlices = 16, UINT thetaSlices = 16);
	static std::string MakeCylinder(VertexFactory& vertices, UINT radialSlices = 16, float height = 1.f);
};

