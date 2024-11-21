#pragma once
#include "Core.h"
#include "Graphics/Bindables/IBindable.h"
#include "Graphics/Resources/ShaderResource.h"

class ShaderResource;

// in general, shit would kinda work like [get has optional override to set data for actual textures?]

// PASS 1: Write to curvature
// Resource curvature = ResourcePool::Get("curvature");
// RenderTargetView* rtv = ResourcePool::Resolve<RenderTargetView>(curvature);
// rtv->Bind();

// PASS 2: Read from curvature
// Resource curvature = ResourcePool::Get("curvature");
// ShaderResourceView* srv = ResourcePool::Resolve<ShaderResourceView>(curvature);
// srv->Bind();

// we can also prolly auto-clear render targets at the end ClearRenderTargetView()


namespace RenderResource
{
	enum class ESRVBindStage : UINT
	{
		Pixel,
		Vertex,
		VertexAndPixel
	};

	class RENDERENGINE_API ResourceView : public IBindable
	{
	public:
		ResourceView(ShaderResource* resource)
			: pResource(resource), m_resID(resource->GetUID())
		{}

	protected:
		std::string m_resID;
		ShaderResource* pResource;
	};

	class RENDERENGINE_API ShaderResourceView : public ResourceView
	{
	public:
		// Init from known resource
		ShaderResourceView(ShaderResource* resource, ESRVBindStage bindStage, UINT slot = 0u);

		void Bind() noexcept override;

		static std::shared_ptr<ShaderResourceView> Resolve(ShaderResource* resource, ESRVBindStage bindStage, UINT slot = 0u);
		static std::string GenerateUID(const ShaderResource* resource, ESRVBindStage bindStage, UINT slot = 0u);

		std::string GetUID() const noexcept override;

	protected:
		ESRVBindStage m_BindStage;
		UINT m_Slot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;
	};

	class RENDERENGINE_API RenderTargetView : public ResourceView
	{
	public:
		RenderTargetView(ShaderResource* resource);

		void Bind() noexcept override;

		static std::shared_ptr<RenderTargetView> Resolve(ShaderResource* resource);
		static std::string GenerateUID(const ShaderResource* resource);

		ID3D11RenderTargetView* GetRTV() const { return pRTV.Get(); }
		std::string GetUID() const noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
	};

	class RENDERENGINE_API DepthStencilView : public ResourceView
	{
	public:
		DepthStencilView(ShaderResource* resource);

		void Bind() noexcept override;

		static std::shared_ptr<DepthStencilView> Resolve(ShaderResource* resource);
		static std::string GenerateUID(const ShaderResource* resource);

		ID3D11DepthStencilView* GetDSV() const { return pDSV.Get(); }
		std::string GetUID() const noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	};
}
