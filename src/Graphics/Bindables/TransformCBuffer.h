﻿#pragma once
#include "IBindable.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>
#include <memory>

// Forward declaration
class IDrawable;

// TODO: Incomplete
namespace RenderResource
{
    
    /// @brief  Only having 1 transform constant buffer, and we just update it before each draw
    class TransformCBuffer : public IBindable
    {
    protected:
        struct Transforms
        {
            DirectX::XMMATRIX modelMatrix;
            DirectX::XMMATRIX modelView;
            DirectX::XMMATRIX modelViewProj;
        };
        
    public:
        TransformCBuffer(Graphics& gfx, const IDrawable& Parent, UINT slot = 0u);
        void Bind(Graphics& gfx) noexcept override;
    protected:
        void UpdateBind_Internal(Graphics& gfx, const Transforms& tf) noexcept;
        Transforms GetTransforms(Graphics& gfx) noexcept;

    private:
        const IDrawable& m_Parent;
        /// @brief  Static to only have 1 version thats ever created, and we just update the transform matrices per draw instead of creating a VCB for each drawable
        static std::unique_ptr<VertexConstantBuffer<Transforms>> pVCbuf;
    };
}
