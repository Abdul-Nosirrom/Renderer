#include "PrimitiveGeometry.h"
#include <random>
#include <iostream>

using namespace RenderResource;

PrimitiveDrawable::PrimitiveDrawable(Graphics& gfx)
{
	InitMesh(gfx, PrimitiveMesh::EType::Cube);
}

PrimitiveDrawable::PrimitiveDrawable(Graphics& gfx, PrimitiveMesh::EType MeshType)
{
	InitMesh(gfx, MeshType);
}

void PrimitiveDrawable::InitMesh(Graphics& gfx, PrimitiveMesh::EType MeshType)
{
	PrimitiveMesh MeshObj(MeshType);
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


	// Test code, base pos
	/*std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> posDist(0, 50); // distribution in range [1, 6]
	std::uniform_int_distribution<std::mt19937::result_type> rotDist(0, 360); // distribution in range [1, 6]
	std::uniform_int_distribution<std::mt19937::result_type> rotvelDist(1, 4); // rotational vel distribution in range 


	SetPos({ (float)posDist(rng) - 25.f, (float)posDist(rng) - 25.f, (float)posDist(rng) - 25.f });
	SetRot({ (float)rotDist(rng), (float)rotDist(rng), (float)rotDist(rng) });

	m_RotVelocity = { (float)rotvelDist(rng)*0.f, (float)rotvelDist(rng) - 2.5f, (float)rotvelDist(rng)*0.f };*/
	m_Pos = { 0.f, 0.f, 5.f };
	m_RotVelocity = { 0.f, 90.f, 0.f };
}


Matrix4x4 PrimitiveDrawable::GetTransformMatrix() const noexcept
{
	const float accumTime = timeAccum.timedata[0];
	Vector3 WorldRot(m_RotVelocity * accumTime);
	auto Transform = MatrixConstruction::Homogenize(MatrixConstruction::RotationMatrix(WorldRot)) * MatrixConstruction::TransformationMatrix(m_Pos, m_Rot, Vector3::Ones());
	return Transform;
}

void PrimitiveDrawable::Update(Graphics& gfx, float DeltaTime)
{
	timeAccum.timedata[0] += DeltaTime;
	GetBindable<PixelConstantBuffer<TimeCBufData>>()->Update(gfx, timeAccum);
}