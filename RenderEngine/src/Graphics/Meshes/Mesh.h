#pragma once

#include "Graphics/Bindables/VertexBuffer.h"
#include <vector>
#include "GeometryFactory.h"

class aiMesh;
enum class GeometryFactory::EType : UINT;

/// @brief	Base class for any object that requires a vertex and index buffer
class Mesh
{
	friend class MeshEntity;
public:
	Mesh() = default;
	~Mesh() = default;
	Mesh(const Mesh&) = delete;

	/// @brief	Load mesh from model file
	Mesh(const aiMesh* inPath); // Mesh from model path
	/// @brief	Generate primitive geometry mesh
	Mesh(GeometryFactory::EType primitiveType);

	Mesh& operator=(const Mesh&) = delete;

	const RenderResource::VertexFactory& GetVertices() const { return m_Vertices; }
	const std::vector<unsigned short>& GetIndices() const { return m_Indices; }

	const std::string& GetPoolTag() const { return m_GeometryTag;  }

protected:
	RenderResource::VertexFactory m_Vertices;
	std::vector<unsigned short> m_Indices;
	std::string m_GeometryTag;
	// Have a transform here for stuff like "radius" instead of baking it into the mesh?

	// this class is where we'd store half edges (?) since it contains the "mesh data"
};

