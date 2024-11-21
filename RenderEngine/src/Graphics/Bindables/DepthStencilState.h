//
// Created by Abdulrahmen on 11/20/2024.
//
#pragma once
#include "Core.h"
#include "IBindable.h"
#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"

namespace RenderResource
{
	class RENDERENGINE_API DepthStencilState : public IBindable
	{
	public:
		DepthStencilState(D3D11_DEPTH_STENCIL_DESC desc)
			: m_desc(desc)
		{
			INFOMAN();
			GFX_THROW_INFO(GetDevice()->CreateDepthStencilState(&desc, &pDepthStencilState));
		}

		void Bind() noexcept override
		{
			GetContext()->OMSetDepthStencilState(pDepthStencilState.Get(), 0);
		}

		static std::shared_ptr<DepthStencilState> Resolve(D3D11_DEPTH_STENCIL_DESC desc)
		{
			return BindablePool::Resolve<DepthStencilState>(desc);
		}
		static std::string GenerateUID(D3D11_DEPTH_STENCIL_DESC desc)
		{
			std::stringstream ss;
			ss << "DepthStencilState::" << desc.DepthEnable << "::" << desc.StencilEnable;
			return ss.str();
		}

		std::string GetUID() const noexcept override { return GenerateUID(m_desc); }

	private:
		D3D11_DEPTH_STENCIL_DESC m_desc;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	};
}
