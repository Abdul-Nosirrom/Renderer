#include "GeometryFactory.h"
#include <format>
#include "Primitives/VertexFactory.h"
#include "Graphics/Bindables/VertexBuffer.h"

using namespace RenderResource;

std::string GeometryFactory::MakeCube(VertexFactory& vertexFactory)
{
	// Reset
	std::vector<Vertex> vertices;
	std::vector<Face> indices;

	// Split normals, so we dupli

	// Vertices
	{
		// Front Face
		{
			Vertex vert;
			vert.SetNormal({ 0, 0, 1 });
			vert.SetColor({ 0, 0, 1 });

			vert.SetPosition({ -1, -1, 1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, -1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, 1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, 1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.push_back(vert);
		}

		// Back Face
		{
			Vertex vert;
			vert.SetNormal({ 0, 0, -1 });
			vert.SetColor({ 0, 0, 0.5 });

			vert.SetPosition({ 1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.push_back(vert);
		}

		// Right Face
		{
			Vertex vert;
			vert.SetNormal({ 1, 0, 0 });
			vert.SetColor({ 1, 0, 0 });

			vert.SetPosition({ 1, -1, 1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, 1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.push_back(vert);
		}

		// Left Face
		{
			Vertex vert;
			vert.SetNormal({ -1, 0, 0 });
			vert.SetColor({ 0.5, 0, 0 });

			vert.SetPosition({ -1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, -1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, 1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.push_back(vert);
		}

		// Top Face
		{
			Vertex vert;
			vert.SetNormal({ 0, 1, 0 });
			vert.SetColor({ 0, 1, 0 });

			vert.SetPosition({ -1, 1, 1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, 1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.push_back(vert);
		}

		// Bottom Face
		{
			Vertex vert;
			vert.SetNormal({ 0, -1, 0 });
			vert.SetColor({ 0, 0.5, 0 });

			vert.SetPosition({ -1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.push_back(vert);

			vert.SetPosition({ -1, -1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.push_back(vert);

			vert.SetPosition({ 1, -1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.push_back(vert);
		}
	}

	for (UINT faceIDX = 0; faceIDX < 6; faceIDX++)
	{
		UINT faceOffset = faceIDX * 4;

		// Tri 1
		indices.push_back({{faceOffset, faceOffset + 1, faceOffset + 2}});
		// Tri 2
		indices.push_back({{faceOffset + 1, faceOffset + 3, faceOffset + 2}});
	}

	vertexFactory.Init(vertices, indices);

	return "$cube.";
}

std::string GeometryFactory::MakePlane(VertexFactory& vertexFactory, UINT tesseleation)
{
	// Reset
	std::vector<Vertex> vertices;
	std::vector<Face> indices;

	const auto v2i = [tesseleation](UINT x, UINT z)
		{
			return (unsigned short)(x * tesseleation + z);
		};

	// On the XZ plane [-1, 1]
	for (UINT Ix = 0; Ix < tesseleation; Ix++)
	{
		const float xPerc = Ix * 1.f / float(tesseleation-1);
		const float X = 1.f * xPerc - 1.f * (1.f - xPerc); // lerp
		for (UINT Iz = 0; Iz < tesseleation; Iz++)
		{
			const float zPerc = Iz * 1.f / float(tesseleation-1);
			const float Z = 1.f * zPerc - 1.f * (1.f - zPerc); // lerp

			Vertex vert;
			vert.SetPosition({ X, 0.f, Z });
			vert.SetUV({ xPerc, zPerc });
			vert.SetNormal({ 0, 1, 0 });
			vertices.push_back(vert);

			// 2D -> 1D map from Ix, Iz [row, column]
			unsigned short idx = v2i(Ix, Iz);
			unsigned short idxAbove = v2i(Ix + 1, Iz);
			unsigned short idxNeighbor = v2i(Ix, Iz + 1);
			unsigned short idxAboveNeighbor = v2i(Ix + 1, Iz + 1);

			// These are handled by neighbors
			if (Ix != tesseleation-1 && Iz != tesseleation-1)
			{
				// tri 1
				indices.push_back({{idx, idxNeighbor, idxAboveNeighbor}});
				// tri 2
				indices.push_back({{idxAbove, idx, idxAboveNeighbor}});
			}
		}
	}

	vertexFactory.Init(vertices, indices);

	return std::format("$plane.{}", tesseleation);
}

std::string GeometryFactory::MakeSphere(VertexFactory& vertexFactory, UINT phiSlices, UINT thetaSlices)
{
	// Reset
	std::vector<Vertex> vertices;
	std::vector<Face> indices;

	for (UINT Iphi = 0; Iphi < phiSlices; Iphi++)
	{
		const float phiPercent = Iphi * 1.f / (phiSlices - 1.f);
		const float phi = phiPercent * PI;

		const float cosPhi = cos(phi);
		const float sinPhi = sin(phi);

		for (UINT Itheta = 0; Itheta < thetaSlices; Itheta++)
		{
			const float thetaPercent = Itheta * 1.f / (thetaSlices - 1.f);
			const float theta = Itheta * 2 * PI / thetaSlices;

			const float cosTheta = cos(theta);
			const float sinTheta = sin(theta);

			const float Z = sin(phi) * cos(theta);
			const float X = sin(phi) * sin(theta);
			const float Y = cos(phi);

			Vertex vert;
			vert.SetPosition({ X, Y, Z });
			vert.SetNormal({ X, Y, Z });
			vert.SetUV({ Itheta * 1.f / thetaSlices, Iphi * 1.f / phiSlices });
			vertices.push_back(vert);

			// define indices
			UINT i1 = Iphi * thetaSlices + Itheta;
			UINT i2 = (Iphi + 1) * thetaSlices + Itheta;
			UINT i3 = Iphi * thetaSlices + (Itheta + 1);
			UINT i4 = (Iphi+1) * thetaSlices + (Itheta+1);

			// tri 1
			indices.push_back({{i1, i2, i3}});
			// tri 2
			indices.push_back({{i3, i2, i4}});
		}
	}

	vertexFactory.Init(vertices, indices);

	return std::format("$sphere.{}.{}", phiSlices, thetaSlices);
}

std::string GeometryFactory::MakeCylinder(VertexFactory& vertexFactory, UINT radialSlices, float height)
{
	// Reset
	std::vector<Vertex> vertices;
	std::vector<Face> indices;

	// Cap vertexFactory
	Vertex capVert;
	capVert.SetUV({ 0.5, 0.5 });
	// Bottom Cap
	capVert.SetPosition({ 0, -height/2.f, 0 });
	capVert.SetNormal({ 0, -1, 0});
	vertices.push_back(capVert);
	// Top Cap
	capVert.SetPosition({ 0, height/2.f, 0 });
	capVert.SetNormal({ 0, 1, 0 });
	vertices.push_back(capVert);
	unsigned short idxOffset = 2;

	// Base body
	for (UINT Iheight = 0; Iheight < 2; Iheight++)
	{
		const float Y = Iheight * height - height/2.f;

		for (UINT Itheta = 0; Itheta < radialSlices; Itheta++)
		{
			const float thetaPercent = Itheta * 1.f / (radialSlices - 1.f);
			const float theta = 2 * PI * thetaPercent;

			const float Z = cos(theta);
			const float X = sin(theta);

			Vertex vert;
			vert.SetPosition({ X, Y, Z });
			vert.SetNormal({ X, 0.f, Z });
			vert.SetUV({ thetaPercent, float(Iheight) });
			vertices.push_back(vert);

			unsigned short radialIdx = Itheta + 2; // Accounting for offset from cap vertexFactory

			// Radial Regular Verts
			if (Iheight == 0)
			{
				// tri 1
				indices.push_back({{radialIdx, radialIdx + 1u, radialIdx + radialSlices}});
				// tri 2
				indices.push_back({{radialIdx + 1u, radialIdx + 1 + radialSlices, radialIdx + radialSlices}});
			}

			idxOffset++;
		}
	}

	// Cap Split
	for (UINT Iheight = 0; Iheight < 2; Iheight++)
	{
		const float Y = Iheight * height - height / 2.f;

		for (UINT Itheta = 0; Itheta < radialSlices; Itheta++)
		{
			const float thetaPercent = Itheta * 1.f / (radialSlices - 1.f);
			const float theta = 2 * PI * thetaPercent;

			const float Z = cos(theta);
			const float X = sin(theta);

			Vertex vert;
			vert.SetPosition({ X, Y, Z });
			vert.SetNormal({ 0, Iheight == 0 ? -1.f : 1.f, 0 });
			vert.SetUV({ (X + 1.f)/2.f, (Z + 1.f)/2.f});
			vertices.push_back(vert);

			unsigned short radialOffset = radialSlices * Iheight + idxOffset; // Accounting for offset from cap vertexFactory

			unsigned short idx1 = radialOffset + Itheta ;
			unsigned short idx2 = Iheight;
			unsigned short idx3 = Itheta == radialSlices - 1 ? radialOffset : radialOffset + Itheta + 1;

			// Different winding number so have to split
			if (Iheight == 0) // Bottom
			{
				indices.push_back({{idx1, idx2, idx3}});
			}
			else // Top
			{
				indices.push_back({{idx1, idx3, idx2}});
			}
		}
	}

	vertexFactory.Init(vertices, indices);
	
	return std::format("$plane.{}.{}", radialSlices, height);
}
