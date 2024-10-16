#include "TransformCBuffer.h"

#include "Graphics/Types/IDrawable.h"
#include "Scene/Scene.h"

using namespace RenderResource;

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVCbuf; 

TransformCBuffer::TransformCBuffer(Graphics& gfx, const IDrawable& Parent, UINT slot)
    : m_Parent(Parent)
{
    // Could've been initialized already
    if (!pVCbuf)
    {
        pVCbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
    }
}

void TransformCBuffer::Bind(Graphics& gfx) noexcept
{
    UpdateBind_Internal(gfx, GetTransforms(gfx));
}

void TransformCBuffer::UpdateBind_Internal(Graphics& gfx, const Transforms& tf) noexcept
{
    pVCbuf->Update(gfx, tf);
    pVCbuf->Bind(gfx);
}

TransformCBuffer::Transforms TransformCBuffer::GetTransforms(Graphics& gfx) noexcept
{
    const auto M = m_Parent.GetTransformMatrix();
    const auto V = Scene::Get().GetViewMatrix();
    const auto P = gfx.GetProjection();

    const auto modelViewProj = (P * V * M);
    const auto modelView = (V * M);

    // transpose before sending off to HLSL
	return { M, modelView, modelViewProj};
}
