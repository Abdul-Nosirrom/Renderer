#pragma once

#include <vector>
#include <d3d11.h>
#include <string>

namespace RenderResource
{
	class VertexFactory;
}

class GeometryFactory
{
public:
	enum class EType : UINT
	{
		Cube, Plane, Sphere, Cylinder
	};

	static const char* MakeCube(RenderResource::VertexFactory& vertices, std::vector<unsigned short>& indices);
	static const char* MakePlane(RenderResource::VertexFactory& vertices, std::vector<unsigned short>& indices, UINT tesselation = 4);
	static const char* MakeSphere(RenderResource::VertexFactory& vertices, std::vector<unsigned short>& indices, UINT phiSlices = 16, UINT thetaSlices = 16);
	static const char* MakeCylinder(RenderResource::VertexFactory& vertices, std::vector<unsigned short>& indices, UINT radialSlices = 16, float height = 1.f);
};

