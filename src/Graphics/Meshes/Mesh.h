#pragma once

#include "Graphics/Bindables/VertexBuffer.h"
#include <vector>

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	const RenderResource::VertexFactory& GetVertices() const { return m_Vertices; }
	const std::vector<unsigned short>& GetIndices() const { return m_Indices; }

	const char* GetPoolTag() const { return m_GeometryTag;  }

protected:
	RenderResource::VertexFactory m_Vertices;
	std::vector<unsigned short> m_Indices;
	const char* m_GeometryTag;
	// Have a transform here for stuff like "radius" instead of baking it into the mesh?
};

