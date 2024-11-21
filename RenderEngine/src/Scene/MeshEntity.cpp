//
// Created by Abdulrahmen on 9/30/2024.
//

#include "MeshEntity.h"
#include "Graphics/Meshes/Mesh.h"
#include "Graphics/Bindables/BindablesCommon.h"
#include <assimp/scene.h>           // Output data structure

using namespace RenderResource;

MeshEntity::MeshEntity(aiMesh* mesh)
{
	m_Mesh = std::make_unique<Mesh>(mesh);
	InitializeBindables();
}

MeshEntity::MeshEntity(GeometryFactory::EType primitiveType)
{
	m_Mesh = std::make_unique<Mesh>(primitiveType);
	InitializeBindables();
}

void MeshEntity::InitializeBindables()
{
	const VertexFactory& meshDescriptor = m_Mesh->GetVertexFactory();
	const auto geoTag = m_Mesh->GetPoolTag();

	// Geo Buffers
	AddBindable(VertexBuffer::Resolve(geoTag, meshDescriptor));
	AddBindable(IndexBuffer::Resolve(geoTag, meshDescriptor));

	// Shaders
	AddBindable(VertexShader::Resolve("shaders/Shaders.hlsl"));

	AddBindable(PixelShader::Resolve("shaders/Shaders.hlsl"));

	// Time constant buffer
	//AddBindable(PixelConstantBuffer<TimeCBufData>::Resolve(gfx, 1));

	// Bind Texture To Pixel Shader
	// AddBindable(Texture::Resolve("resources/TextureTest.png", ETextureBindStage::Pixel, 0));
	// AddBindable(Sampler::Resolve(Sampler::Type::Anisotropic, ETextureBindStage::Pixel, 0));
	//
	// AddBindable(Texture::Resolve("resources/TextureTest.png", ETextureBindStage::Vertex, 0));
	// AddBindable(Sampler::Resolve(Sampler::Type::Anisotropic, ETextureBindStage::Vertex, 0));

	// Shader Data
	AddBindable(InputLayout::Resolve());

	// Topology
	AddBindable(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Constant Buffers
	AddBindable(std::make_shared<TransformCBuffer>(*this));
}

void MeshEntity::OnRender() const
{
	Draw();

	for (const auto& childNode : m_childNodes)
	{
		childNode->OnRender();
	}
}

const std::string& MeshEntity::GetName() const
{
	return m_Mesh->m_GeometryTag;
}
