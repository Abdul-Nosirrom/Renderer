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
	auto pVS = VertexShader::Resolve(gfx, "shaders/Shaders.hlsl");
	auto pVSByteCode = pVS->GetBytecode(); // Retrieve to feed input input layout
	AddBindable(std::move(pVS));

	AddBindable(PixelShader::Resolve(gfx, "shaders/Shaders.hlsl"));

	// Time constant buffer
	AddBindable(VertexConstantBuffer<TimeCBufData>::Resolve(gfx, 1));


	// Shader Data
	AddBindable(InputLayout::Resolve(gfx, pVSByteCode));

	// Topology
	AddBindable(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Constant Buffers
	AddBindable(std::make_shared<TransformCBuffer>(gfx, *this));


	// Test code, base pos
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> posDist(0, 50); // distribution in range [1, 6]
	std::uniform_int_distribution<std::mt19937::result_type> rotDist(0, 360); // distribution in range [1, 6]
	std::uniform_int_distribution<std::mt19937::result_type> rotvelDist(1, 4); // rotational vel distribution in range 


	SetPos({ (float)posDist(rng) - 25.f, (float)posDist(rng) - 25.f, (float)posDist(rng) - 25.f });
	SetRot({ (float)rotDist(rng), (float)rotDist(rng), (float)rotDist(rng) });

	m_RotVelocity = { (float)rotvelDist(rng)*0.f, (float)rotvelDist(rng) - 2.5f, (float)rotvelDist(rng)*0.f };

}


DirectX::XMMATRIX PrimitiveDrawable::GetTransformMatrix() const noexcept
{
	const float accumTime = timeAccum.timedata[0];
	DirectX::XMFLOAT3 WorldRot = { m_RotVelocity.x * accumTime, m_RotVelocity.y * accumTime, m_RotVelocity.z * accumTime };
	return DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z) * DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z) * DirectX::XMMatrixRotationRollPitchYaw(WorldRot.x, WorldRot.y, WorldRot.z);
}

void PrimitiveDrawable::Update(Graphics& gfx, float DeltaTime)
{
	timeAccum.timedata[0] += DeltaTime;
	GetBindable<VertexConstantBuffer<TimeCBufData>>()->Update(gfx, timeAccum);
}