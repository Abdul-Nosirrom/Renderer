#include "GeometryFactory.h"
#include <format>

#include "Graphics/Bindables/VertexBuffer.h"

using namespace RenderResource;

const char* GeometryFactory::MakeCube(VertexFactory& vertices, std::vector<unsigned short>& indices)
{
	// Reset
	vertices = VertexFactory();
	indices.clear();

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
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.AddVertex(vert);
		}

		// Back Face
		{
			Vertex vert;
			vert.SetNormal({ 0, 0, -1 });
			vert.SetColor({ 0, 0, 0.5 });

			vert.SetPosition({ 1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.AddVertex(vert);
		}

		// Right Face
		{
			Vertex vert;
			vert.SetNormal({ 1, 0, 0 });
			vert.SetColor({ 1, 0, 0 });

			vert.SetPosition({ 1, -1, 1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.AddVertex(vert);
		}

		// Left Face
		{
			Vertex vert;
			vert.SetNormal({ -1, 0, 0 });
			vert.SetColor({ 0.5, 0, 0 });

			vert.SetPosition({ -1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, -1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.AddVertex(vert);
		}

		// Top Face
		{
			Vertex vert;
			vert.SetNormal({ 0, 1, 0 });
			vert.SetColor({ 0, 1, 0 });

			vert.SetPosition({ -1, 1, 1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.AddVertex(vert);
		}

		// Bottom Face
		{
			Vertex vert;
			vert.SetNormal({ 0, -1, 0 });
			vert.SetColor({ 0, 0.5, 0 });

			vert.SetPosition({ -1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			vertices.AddVertex(vert);

			vert.SetPosition({ -1, -1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			vertices.AddVertex(vert);
		}
	}

	for (UINT faceIDX = 0; faceIDX < 6; faceIDX++)
	{
		UINT faceOffset = faceIDX * 4;

		// Tri 1
		indices.push_back(faceOffset);
		indices.push_back(faceOffset + 1);
		indices.push_back(faceOffset + 2);

		// Tri 2
		indices.push_back(faceOffset + 1);
		indices.push_back(faceOffset + 3);
		indices.push_back(faceOffset + 2);
	}

	return "$cube.";
}

const char* GeometryFactory::MakePlane(VertexFactory& vertices, std::vector<unsigned short>& indices, UINT tesseleation)
{
	// Reset
	vertices = VertexFactory();
	indices.clear();

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
			vertices.AddVertex(vert);

			// 2D -> 1D map from Ix, Iz [row, column]
			unsigned short idx = v2i(Ix, Iz);
			unsigned short idxAbove = v2i(Ix + 1, Iz);
			unsigned short idxNeighbor = v2i(Ix, Iz + 1);
			unsigned short idxAboveNeighbor = v2i(Ix + 1, Iz + 1);

			// These are handled by neighbors
			if (Ix != tesseleation-1 && Iz != tesseleation-1)
			{
				// tri 1
				indices.push_back(idx);
				indices.push_back(idxNeighbor);
				indices.push_back(idxAboveNeighbor);

				// tri 2
				indices.push_back(idxAbove);
				indices.push_back(idx);
				indices.push_back(idxAboveNeighbor);
			}
		}
	}

	return std::format("$plane.{}", tesseleation).c_str();
}

const char* GeometryFactory::MakeSphere(VertexFactory& vertices, std::vector<unsigned short>& indices, UINT phiSlices, UINT thetaSlices)
{
	// Reset
	vertices = VertexFactory();
	indices.clear();

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
			vertices.AddVertex(vert);

			// tri 1
			indices.push_back(Iphi * thetaSlices + Itheta);
			indices.push_back((Iphi+1) * thetaSlices + Itheta);
			indices.push_back(Iphi * thetaSlices + (Itheta+1));
			// tri 2
			indices.push_back(Iphi * thetaSlices + (Itheta+1));
			indices.push_back((Iphi+1) * thetaSlices + Itheta);
			indices.push_back((Iphi+1) * thetaSlices + (Itheta+1));
		}
	}

	return std::format("$sphere.{}.{}", phiSlices, thetaSlices).c_str();
}

const char* GeometryFactory::MakeCylinder(VertexFactory& vertices, std::vector<unsigned short>& indices, UINT radialSlices, float height)
{
	// Reset
	vertices = VertexFactory();
	indices.clear();

	// Cap vertices
	Vertex capVert;
	capVert.SetUV({ 0.5, 0.5 });
	// Bottom Cap
	capVert.SetPosition({ 0, -height/2.f, 0 });
	capVert.SetNormal({ 0, -1, 0});
	vertices.AddVertex(capVert);
	// Top Cap
	capVert.SetPosition({ 0, height/2.f, 0 });
	capVert.SetNormal({ 0, 1, 0 });
	vertices.AddVertex(capVert);
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
			vertices.AddVertex(vert);

			unsigned short radialIdx = Itheta + 2; // Accounting for offset from cap vertices

			// Radial Regular Verts
			if (Iheight == 0)
			{
				// tri 1
				indices.push_back(radialIdx);
				indices.push_back(radialIdx + 1);
				indices.push_back(radialIdx + radialSlices);
				// tri 2
				indices.push_back(radialIdx + 1);
				indices.push_back(radialIdx + 1 + radialSlices);
				indices.push_back(radialIdx + radialSlices);
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
			vertices.AddVertex(vert);

			unsigned short radialOffset = radialSlices * Iheight + idxOffset; // Accounting for offset from cap vertices

			unsigned short idx1 = radialOffset + Itheta ;
			unsigned short idx2 = Iheight;
			unsigned short idx3 = Itheta == radialSlices - 1 ? radialOffset : radialOffset + Itheta + 1;

			// Different winding number so have to split
			if (Iheight == 0) // Bottom
			{
				indices.push_back(idx1);
				indices.push_back(idx2);
				indices.push_back(idx3);
			}
			else // Top
			{
				indices.push_back(idx1);
				indices.push_back(idx3);
				indices.push_back(idx2);
			}
		}
	}

	return std::format("$plane.{}.{}", radialSlices, height).c_str();
}
