//
// Created by Abdulrahmen on 9/30/2024.
//

#include "MeshEntity.h"
#include "Graphics/Meshes/Mesh.h"
#include "Graphics/Bindables/BindablesCommon.h"
#include <assimp/scene.h>           // Output data structure

using namespace RenderResource;

MeshEntity::MeshEntity(Graphics& gfx, aiMesh* mesh)
{
	m_Mesh = std::make_unique<Mesh>(mesh);
	InitializeBindables(gfx);
}

MeshEntity::MeshEntity(Graphics &gfx, GeometryFactory::EType primitiveType)
{
	m_Mesh = std::make_unique<Mesh>(primitiveType);
	InitializeBindables(gfx);
}

void MeshEntity::InitializeBindables(Graphics& gfx)
{
	VertexFactory meshDescriptor = m_Mesh->GetVertices();
	auto idxData = m_Mesh->GetIndices();
	const auto geoTag = m_Mesh->GetPoolTag();

	// Geo Buffers
	AddBindable(VertexBuffer::Resolve(gfx, geoTag, meshDescriptor));
	AddBindable(IndexBuffer::Resolve(gfx, geoTag, idxData));

	// Shaders
	auto pVS = VertexShader::Resolve(gfx, "shaders/Shaders.hlsl");
	auto pVSByteCode = pVS->GetBytecode(); // Retrieve to feed input input layout
	AddBindable(std::move(pVS));

	AddBindable(PixelShader::Resolve(gfx, "shaders/Shaders.hlsl"));

	// Time constant buffer
	//AddBindable(PixelConstantBuffer<TimeCBufData>::Resolve(gfx, 1));

	// Bind Texture To Pixel Shader
	AddBindable(Texture::Resolve(gfx, "resources/TextureTest.png", ETextureBindStage::Pixel, 0));
	AddBindable(Sampler::Resolve(gfx, Sampler::Type::Anisotropic, ETextureBindStage::Pixel, 0));

	AddBindable(Texture::Resolve(gfx, "resources/TextureTest.png", ETextureBindStage::Vertex, 0));
	AddBindable(Sampler::Resolve(gfx, Sampler::Type::Anisotropic, ETextureBindStage::Vertex, 0));

	// Shader Data
	AddBindable(InputLayout::Resolve(gfx, pVSByteCode));

	// Topology
	AddBindable(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Constant Buffers
	AddBindable(std::make_shared<TransformCBuffer>(gfx, *this));
}

void MeshEntity::OnRender(Graphics &gfx) const
{
	Draw(gfx);

	for (const auto& childNode : m_childNodes)
	{
		childNode->OnRender(gfx);
	}
}
