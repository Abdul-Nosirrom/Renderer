﻿#pragma once
#include "Core.h"
#include "IDrawable.h"
#include "../Meshes/PrimitiveMesh.h"
#include "Graphics/Bindables/BindablesCommon.h"


class RENDERENGINE_API PrimitiveDrawable : public IDrawable
{
public:
    PrimitiveDrawable() = default;
    PrimitiveDrawable(Graphics& gfx);
    PrimitiveDrawable(Graphics& gfx, PrimitiveMesh::EType MeshType);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

    void SetPos(DirectX::XMFLOAT3 InPos) noexcept { m_Pos = InPos; }
    void SetRot(DirectX::XMFLOAT3 InRot) noexcept { m_Rot = InRot; }

    virtual void Update(Graphics& gfx, float DeltaTime) override;
    
private:
    void InitMesh(Graphics& gfx, PrimitiveMesh::EType MeshType);

    DirectX::XMFLOAT3 m_Pos = {0.f, 0.f, 0.f};
    DirectX::XMFLOAT3 m_Rot = {0.f, 0.f, 0.f};

    // Temp code
    DirectX::XMFLOAT3 m_RotVelocity = { 0.f, 0.f, 0.f };


	struct TimeCBufData
	{
		float timedata[4] = { 0.f, 0.f, 0.f, 0.f };
	};
	TimeCBufData timeAccum;
	std::shared_ptr<RenderResource::VertexConstantBuffer<TimeCBufData>> TimeCBuf;

};
    
