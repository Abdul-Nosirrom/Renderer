#include "RenderResourcePool.h"

#include "Resources/Views/ResourceView.h"

using namespace RenderResource;

void BindablePool::ClearRenderTargetViews(const Vector4& clearColor)
{
	for (auto bind : Get().binds)
	{
		auto bindable = bind.second;
		if (auto rtv = dynamic_cast<RenderTargetView*>(bindable.get()))
		{
			Renderer::GetContext()->ClearRenderTargetView(rtv->GetRTV(), clearColor.data());
		}
	}
}

void BindablePool::ClearDepthStencilViews(float depthVal, float stencilVal)
{
	for (auto bind : Get().binds)
	{
		auto bindable = bind.second;
		if (auto dsv = dynamic_cast<DepthStencilView*>(bindable.get()))
		{
			Renderer::GetContext()->ClearDepthStencilView(dsv->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depthVal, stencilVal);
		}
	}
}