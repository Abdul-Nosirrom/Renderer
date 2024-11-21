#pragma once
#include "IBindable.h"
#include "ConstantBuffers.h"
#include "Math.h"
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
            Matrix4x4 modelMatrix;
            Matrix4x4 modelView;
            Matrix4x4 modelViewProj;
        };
        
    public:
        TransformCBuffer(const IDrawable& Parent, UINT slot = 0u);
        void Bind() noexcept override;
    protected:
        void UpdateBind_Internal(const Transforms& tf) noexcept;
        Transforms GetTransforms() noexcept;

    private:
        const IDrawable& m_Parent;
        /// @brief  Static to only have 1 version thats ever created, and we just update the transform matrices per draw instead of creating a VCB for each drawable
        static std::unique_ptr<VertexConstantBuffer<Transforms>> pVCbuf;
    };
}
