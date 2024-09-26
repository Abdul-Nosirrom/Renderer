#include "ModelDrawable.h"
#include "../Meshes/Model.h"

using namespace RenderResource;

ModelDrawable::ModelDrawable(Graphics& gfx, const std::string& modelPath)
{
	Model MeshObj(modelPath);
	VertexFactory meshDescriptor = MeshObj.GetVertices();
	auto idxData = MeshObj.GetIndices();
	const auto geoTag = MeshObj.GetPoolTag();

	// Geo Buffers
	AddBindable(VertexBuffer::Resolve(gfx, geoTag, meshDescriptor));
	AddBindable(IndexBuffer::Resolve(gfx, geoTag, idxData));

	// Shaders
	auto pVS = VertexShader::Resolve(gfx, "./../../../shaders/Shaders.hlsl");
	auto pVSByteCode = pVS->GetBytecode(); // Retrieve to feed input input layout
	AddBindable(std::move(pVS));

	AddBindable(PixelShader::Resolve(gfx, "./../../../shaders/Shaders.hlsl"));

	// Time constant buffer
	AddBindable(PixelConstantBuffer<TimeCBufData>::Resolve(gfx, 1));

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
