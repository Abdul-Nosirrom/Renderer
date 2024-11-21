#include "Sampler.h"
#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"

namespace wrl = Microsoft::WRL;
using namespace RenderResource;

Sampler::Sampler(Type type, ESRVBindStage bindStage, UINT slot)
	: m_Type(type), m_Slot(slot), m_BindStage(bindStage)
{
	INFOMAN();

	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
	samplerDesc.Filter = [type]() // this is a cool lambda thing i didn't know existed huh
	{
		switch (type)
		{
		case Type::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
		case Type::Point: return D3D11_FILTER_MIN_MAG_MIP_POINT;
		default:
		case Type::Bilinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
	}();

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

	GFX_THROW_INFO(GetDevice()->CreateSamplerState(&samplerDesc, &pSampler));
}

void Sampler::Bind() noexcept
{
	switch (m_BindStage)
	{
	case ESRVBindStage::Pixel:
		GetContext()->PSSetSamplers(m_Slot, 1u, pSampler.GetAddressOf()); break;
	case ESRVBindStage::Vertex:
		GetContext()->VSSetSamplers(m_Slot, 1u, pSampler.GetAddressOf()); break;
	case ESRVBindStage::VertexAndPixel:
		GetContext()->PSSetSamplers(m_Slot, 1u, pSampler.GetAddressOf());
		GetContext()->VSSetSamplers(m_Slot, 1u, pSampler.GetAddressOf()); break;
	}
}

std::shared_ptr<Sampler> Sampler::Resolve(Type type, ESRVBindStage bindStage, UINT slot)
{
	return BindablePool::Resolve<Sampler>(type, bindStage, slot);
}

std::string Sampler::GenerateUID(Type type, ESRVBindStage bindStage, UINT slot)
{
	using namespace std::string_literals;
	const std::string stage = bindStage == ESRVBindStage::Vertex ? "vertex" : "pixel";
	return typeid(Sampler).name() + "#"s + std::to_string((int)type) + "@"s + std::to_string(slot) + stage;
}

std::string Sampler::GetUID() const noexcept
{
	return GenerateUID(m_Type, m_BindStage, m_Slot);
}
