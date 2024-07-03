#include "PrimitiveMesh.h"
#include <format>


using namespace RenderResource;

void PrimitiveMesh::MakeCube()
{
	m_GeometryTag = "$cube.";

	// Reset
	m_Vertices = VertexFactory();
	m_Indices.clear();

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
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			m_Vertices.AddVertex(vert);
		}

		// Back Face
		{
			Vertex vert;
			vert.SetNormal({ 0, 0, -1 });
			vert.SetColor({ 0, 0, 0.5 });

			vert.SetPosition({ 1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			m_Vertices.AddVertex(vert);
		}

		// Right Face
		{
			Vertex vert;
			vert.SetNormal({ 1, 0, 0 });
			vert.SetColor({ 1, 0, 0 });

			vert.SetPosition({ 1, -1, 1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			m_Vertices.AddVertex(vert);
		}

		// Left Face
		{
			Vertex vert;
			vert.SetNormal({ -1, 0, 0 });
			vert.SetColor({ 0.5, 0, 0 });

			vert.SetPosition({ -1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, -1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			m_Vertices.AddVertex(vert);
		}

		// Top Face
		{
			Vertex vert;
			vert.SetNormal({ 0, 1, 0 });
			vert.SetColor({ 0, 1, 0 });

			vert.SetPosition({ -1, 1, 1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, 1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, 1, -1 }); // Top Left
			vert.SetUV({ 0, 0 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, 1, -1 }); // Top Right
			vert.SetUV({ 1, 0 });
			m_Vertices.AddVertex(vert);
		}

		// Bottom Face
		{
			Vertex vert;
			vert.SetNormal({ 0, -1, 0 });
			vert.SetColor({ 0, 0.5, 0 });

			vert.SetPosition({ -1, -1, -1 }); // Bottom Left
			vert.SetUV({ 0, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, -1 }); // Bottom Right
			vert.SetUV({ 1, 1 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ -1, -1, 1 }); // Top Left
			vert.SetUV({ 0, 0 });
			m_Vertices.AddVertex(vert);

			vert.SetPosition({ 1, -1, 1 }); // Top Right
			vert.SetUV({ 1, 0 });
			m_Vertices.AddVertex(vert);
		}
	}

	for (UINT faceIDX = 0; faceIDX < 6; faceIDX++)
	{
		UINT faceOffset = faceIDX * 4;

		// Tri 1
		m_Indices.push_back(faceOffset);
		m_Indices.push_back(faceOffset + 1);
		m_Indices.push_back(faceOffset + 2);

		// Tri 2
		m_Indices.push_back(faceOffset + 1);
		m_Indices.push_back(faceOffset + 3);
		m_Indices.push_back(faceOffset + 2);
	}

}

void PrimitiveMesh::MakePlane(UINT tesseleation)
{
	m_GeometryTag = std::format("$plane.{}", tesseleation).c_str();

	// Reset
	m_Vertices = VertexFactory();
	m_Indices.clear();

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
			m_Vertices.AddVertex(vert);

			// 2D -> 1D map from Ix, Iz [row, column]
			unsigned short idx = v2i(Ix, Iz);
			unsigned short idxAbove = v2i(Ix + 1, Iz);
			unsigned short idxNeighbor = v2i(Ix, Iz + 1);
			unsigned short idxAboveNeighbor = v2i(Ix + 1, Iz + 1);

			// These are handled by neighbors
			if (Ix != tesseleation-1 && Iz != tesseleation-1)
			{
				// tri 1
				m_Indices.push_back(idx);
				m_Indices.push_back(idxNeighbor);
				m_Indices.push_back(idxAboveNeighbor);

				// tri 2
				m_Indices.push_back(idxAbove);
				m_Indices.push_back(idx);
				m_Indices.push_back(idxAboveNeighbor);
			}
		}
	}

}

void PrimitiveMesh::MakeSphere(UINT phiSlices, UINT thetaSlices)
{
	m_GeometryTag = std::format("$sphere.{}.{}", phiSlices, thetaSlices).c_str();

	// Reset
	m_Vertices = VertexFactory();
	m_Indices.clear();

	for (UINT Iphi = 0; Iphi < phiSlices; Iphi++)
	{
		const float phiPercent = Iphi * 1.f / (phiSlices - 1.f);
		const float phi = phiPercent * DirectX::XM_PI;

		const float cosPhi = cos(phi);
		const float sinPhi = sin(phi);

		for (UINT Itheta = 0; Itheta < thetaSlices; Itheta++)
		{
			const float thetaPercent = Itheta * 1.f / (thetaSlices - 1.f);
			const float theta = Itheta * DirectX::XM_2PI / thetaSlices;

			const float cosTheta = cos(theta);
			const float sinTheta = sin(theta);

			const float Z = sin(phi) * cos(theta);
			const float X = sin(phi) * sin(theta);
			const float Y = cos(phi);

			Vertex vert;
			vert.SetPosition({ X, Y, Z });
			vert.SetNormal({ X, Y, Z });
			//vert.SetUV({ Itheta * 1.f / thetaSlices, Iphi * 1.f / phiSlices });
			m_Vertices.AddVertex(vert);

			// tri 1
			m_Indices.push_back(Iphi * thetaSlices + Itheta);
			m_Indices.push_back((Iphi+1) * thetaSlices + Itheta);
			m_Indices.push_back(Iphi * thetaSlices + (Itheta+1));
			// tri 2
			m_Indices.push_back(Iphi * thetaSlices + (Itheta+1));
			m_Indices.push_back((Iphi+1) * thetaSlices + Itheta);
			m_Indices.push_back((Iphi+1) * thetaSlices + (Itheta+1));
		}
	}
}

void PrimitiveMesh::MakeCylinder(UINT radialSlices, float height)
{
	m_GeometryTag = std::format("$plane.{}.{}", radialSlices, height).c_str();

	// Reset
	m_Vertices = VertexFactory();
	m_Indices.clear();

	// Cap vertices
	Vertex capVert;
	capVert.SetUV({ 0.5, 0.5 });
	// Bottom Cap
	capVert.SetPosition({ 0, -height/2.f, 0 });
	capVert.SetNormal({ 0, -1, 0});
	m_Vertices.AddVertex(capVert);
	// Top Cap
	capVert.SetPosition({ 0, height/2.f, 0 });
	capVert.SetNormal({ 0, 1, 0 });
	m_Vertices.AddVertex(capVert);
	unsigned short idxOffset = 2;

	// Base body
	for (UINT Iheight = 0; Iheight < 2; Iheight++)
	{
		const float Y = Iheight * height - height/2.f;

		for (UINT Itheta = 0; Itheta < radialSlices; Itheta++)
		{
			const float thetaPercent = Itheta * 1.f / (radialSlices - 1.f);
			const float theta = DirectX::XM_2PI * thetaPercent;

			const float Z = cos(theta);
			const float X = sin(theta);

			Vertex vert;
			vert.SetPosition({ X, Y, Z });
			vert.SetNormal({ X, 0.f, Z });
			vert.SetUV({ thetaPercent, float(Iheight) });
			m_Vertices.AddVertex(vert);

			unsigned short radialIdx = Itheta + 2; // Accounting for offset from cap vertices

			// Radial Regular Verts
			if (Iheight == 0)
			{
				// tri 1
				m_Indices.push_back(radialIdx);
				m_Indices.push_back(radialIdx + 1);
				m_Indices.push_back(radialIdx + radialSlices);
				// tri 2
				m_Indices.push_back(radialIdx + 1);
				m_Indices.push_back(radialIdx + 1 + radialSlices);
				m_Indices.push_back(radialIdx + radialSlices);
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
			const float theta = DirectX::XM_2PI * thetaPercent;

			const float Z = cos(theta);
			const float X = sin(theta);

			Vertex vert;
			vert.SetPosition({ X, Y, Z });
			vert.SetNormal({ 0, Iheight == 0 ? -1.f : 1.f, 0 });
			vert.SetUV({ X, Z });
			m_Vertices.AddVertex(vert);

			unsigned short radialOffset = radialSlices * Iheight + idxOffset; // Accounting for offset from cap vertices

			unsigned short idx1 = radialOffset + Itheta ;
			unsigned short idx2 = Iheight;
			unsigned short idx3 = Itheta == radialSlices - 1 ? radialOffset : radialOffset + Itheta + 1;

			// Different winding number so have to split
			if (Iheight == 0) // Bottom
			{
				m_Indices.push_back(idx1);
				m_Indices.push_back(idx2);
				m_Indices.push_back(idx3);
			}
			else // Top
			{
				m_Indices.push_back(idx1);
				m_Indices.push_back(idx3);
				m_Indices.push_back(idx2);
			}
		}
	}
}
