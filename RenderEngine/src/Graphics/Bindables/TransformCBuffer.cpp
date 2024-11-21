#include "TransformCBuffer.h"

#include "Graphics/Types/IDrawable.h"
#include "Scene/Scene.h"

using namespace RenderResource;

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVCbuf; 

TransformCBuffer::TransformCBuffer(const IDrawable& Parent, UINT slot)
    : m_Parent(Parent)
{
    // Could've been initialized already
    if (!pVCbuf)
    {
        pVCbuf = std::make_unique<VertexConstantBuffer<Transforms>>(slot);
    }
}

void TransformCBuffer::Bind() noexcept
{
    UpdateBind_Internal(GetTransforms());
}

void TransformCBuffer::UpdateBind_Internal(const Transforms& tf) noexcept
{
    pVCbuf->Update(tf);
    pVCbuf->Bind();
}

TransformCBuffer::Transforms TransformCBuffer::GetTransforms() noexcept
{
    const auto M = m_Parent.GetTransformMatrix();
    const auto V = Scene::Get().GetViewMatrix();
    const auto P = Renderer::GetProjectionMatrix();

    const auto modelViewProj = (P * V * M);
    const auto modelView = (V * M);

    // transpose before sending off to HLSL
	return { M, modelView, modelViewProj};
}
