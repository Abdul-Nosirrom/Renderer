//
// Created by Abdulrahmen on 11/12/2024.
//

#include "ResourceView.h"
#include "Graphics/RenderResourcePool.h"
#include "Errors/GraphicsExceptions.h"

using namespace RenderResource;


////////////////////////////////////////////////////////////////////
/// SRV
///////////////////////////////////////////////////////////////////

ShaderResourceView::ShaderResourceView(ShaderResource* resource, ESRVBindStage bindStage, UINT slot)
	: ResourceView(resource), m_BindStage(bindStage), m_Slot(slot)
{
	INFOMAN();

	D3D11_TEXTURE2D_DESC texDesc = {};
	resource->Texture2D()->GetDesc(&texDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	GFX_THROW_INFO(Renderer::GetDevice()->CreateShaderResourceView(resource->Texture2D().Get(), &srvDesc, &pSRV));

}

void ShaderResourceView::Bind() noexcept
{
	switch (m_BindStage)
	{
		case ESRVBindStage::Pixel:
			Renderer::GetContext()->PSSetShaderResources(m_Slot, 1u, pSRV.GetAddressOf());
			break;
		case ESRVBindStage::Vertex:
			Renderer::GetContext()->VSSetShaderResources(m_Slot, 1u, pSRV.GetAddressOf());
			break;
		case ESRVBindStage::VertexAndPixel:
			Renderer::GetContext()->PSSetShaderResources(m_Slot, 1u, pSRV.GetAddressOf());
			Renderer::GetContext()->VSSetShaderResources(m_Slot, 1u, pSRV.GetAddressOf());
			break;
	}

}

std::shared_ptr<ShaderResourceView> ShaderResourceView::Resolve(ShaderResource* resource,
	ESRVBindStage bindStage, UINT slot)
{
	return BindablePool::Resolve<ShaderResourceView>(resource, bindStage, slot);
}

std::string ShaderResourceView::GenerateUID(const ShaderResource* resource, ESRVBindStage bindStage,
                                            UINT slot)
{
	using namespace std::string_literals;
	const std::string stage = bindStage == ESRVBindStage::Vertex ? "vertex" : (bindStage == ESRVBindStage::Pixel ? "pixel" : "vertex_pixe");
	return typeid(ShaderResourceView).name() + "#"s + resource->GetUID() + "#" + std::to_string(slot) + "#" + stage;
}

std::string ShaderResourceView::GetUID() const noexcept
{
	// SRV_ResourceName
	return GenerateUID(pResource, m_BindStage, m_Slot);
}

////////////////////////////////////////////////////////////////////
/// RENDER TARGET
///////////////////////////////////////////////////////////////////

RenderTargetView::RenderTargetView(ShaderResource* resource)
	: ResourceView(resource)
{
	INFOMAN();

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	D3D11_TEXTURE2D_DESC texDesc = {};
	resource->Texture2D()->GetDesc(&texDesc);
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create a reference to the render target view to access and modify later. This is getting a reference to the back buffer, so we're drawing to that render target
	// which is the back buffer, then upon swapping buffer presenting it (in which the front buffer now becomes the back buffer we draw to next frame)
	GFX_THROW_INFO(Renderer::GetDevice()->CreateRenderTargetView(
		resource->Texture2D().Get(),  /* Buffer in which to receive the render target from */
		&rtvDesc,							/* Config struct to specify how we wanna receive the RTV, default it */
		&pRTV									/* ID3D11 Target to be filled out */
	));
}

void RenderTargetView::Bind() noexcept
{
	Renderer::GetContext()->OMSetRenderTargets(1u, &pRTV, nullptr);
}

std::shared_ptr<RenderTargetView> RenderTargetView::Resolve(ShaderResource* resource)
{
	return BindablePool::Resolve<RenderTargetView>(resource);
}

std::string RenderTargetView::GenerateUID(const ShaderResource* resource)
{
	std::stringstream ss;
	ss << "rtv_" << resource->GetUID();
	return ss.str();
}

std::string RenderTargetView::GetUID() const noexcept
{
	// RTV_ResourceName
	return GenerateUID(pResource);
}

////////////////////////////////////////////////////////////////////
/// DEPTH STENCIL
///////////////////////////////////////////////////////////////////

DepthStencilView::DepthStencilView(ShaderResource* resource)
	: ResourceView(resource)
{
	INFOMAN();

	// In order to bind a texture to the pipeline, need to create a 'view' for that texture
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0u;

	GFX_THROW_INFO(Renderer::GetDevice()->CreateDepthStencilView(resource->Texture2D().Get(), &dsvDesc, &pDSV));

}

void DepthStencilView::Bind() noexcept
{
	// need a render target...
	Renderer::GetContext()->OMSetRenderTargets(1u, nullptr, pDSV.Get());
}

std::shared_ptr<DepthStencilView> DepthStencilView::Resolve(ShaderResource* resource)
{
	return BindablePool::Resolve<DepthStencilView>(resource);
}

std::string DepthStencilView::GenerateUID(const ShaderResource* resource)
{
	// DSV_ResourceName
	std::stringstream ss;
	ss << "dsv_" << resource->GetUID();
	return ss.str();
}

std::string DepthStencilView::GetUID() const noexcept
{
	return GenerateUID(pResource);
}
