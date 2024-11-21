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
	class RENDERENGINE_API BlendState : public IBindable
	{
	public:
		BlendState(D3D11_RENDER_TARGET_BLEND_DESC desc)
			: m_desc(desc)
		{
			INFOMAN();
			D3D11_BLEND_DESC blendStateDesc = {};
			blendStateDesc.RenderTarget[0] = m_desc;
			GFX_THROW_INFO(GetDevice()->CreateBlendState(&blendStateDesc, &pBlendState));
		}

		void Bind() noexcept override
		{
			GetContext()->OMSetBlendState(pBlendState.Get(), nullptr, 0);
		}

		static std::shared_ptr<BlendState> Resolve(D3D11_RENDER_TARGET_BLEND_DESC desc)
		{
			return BindablePool::Resolve<BlendState>(desc);
		}
		static std::string GenerateUID(D3D11_RENDER_TARGET_BLEND_DESC desc)
		{
			// unique identifier from the blend state description
			std::stringstream ss;
			ss << "BlendState::" << desc.BlendEnable << desc.BlendOp << desc.DestBlend << desc.SrcBlend << desc.BlendOpAlpha << desc.SrcBlendAlpha;
			return ss.str();
		}

		std::string GetUID() const noexcept override { return GenerateUID(m_desc); }


	private:
		D3D11_RENDER_TARGET_BLEND_DESC m_desc;
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
	};
}

