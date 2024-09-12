#include "IDrawable.h"

#include "Graphics/Bindables/IBindable.h"
#include "Graphics/Bindables/IndexBuffer.h"

using namespace RenderResource;

void IDrawable::Draw(Graphics& gfx) const
{
    // Bind each bindable then call draw
    for (auto& bindable : m_Bindables)
    {
        bindable->Bind(gfx);
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void IDrawable::AddBindable(std::shared_ptr<IBindable> bindable)
{
    // Ensure that only 1 index buffers been added
    if (typeid(*bindable) == typeid(IndexBuffer))
    {
        assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
        pIndexBuffer = &static_cast<IndexBuffer&>(*bindable);
    }

    m_Bindables.push_back(std::move(bindable));
}
