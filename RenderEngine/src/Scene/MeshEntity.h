#pragma once
#include "Core.h"
#include "EntityNode.h"
#include "Graphics/Meshes/Mesh.h"
#include "Graphics/Meshes/GeometryFactory.h"

//class Mesh;
class aiMesh;

class RENDERENGINE_API MeshEntity : public EntityNode
{
public:
	MeshEntity() = delete;
	MeshEntity(aiMesh* mesh);
	MeshEntity(GeometryFactory::EType primitiveType);
	//~MeshEntity() { m_Mesh->rel; }

	virtual void InitializeBindables();

	void OnRender() const override;
	const std::string& GetName() const override;
	const std::unique_ptr<Mesh>& GetMesh() const { return m_Mesh; }

protected:

	std::unique_ptr<Mesh> m_Mesh;
};
