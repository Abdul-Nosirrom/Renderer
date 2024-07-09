#include "Texture.h"
#include "Errors/GraphicsExceptions.h"
#include "Utility/WICTextureLoader.h"
#include "Graphics/RenderResourcePool.h"

namespace wrl = Microsoft::WRL;
using namespace RenderResource;

//////////////////////////////////////////////////////////////////////////
// Texture
//////////////////////////////////////////////////////////////////////////

Texture::Texture(Graphics& gfx, const std::string& filePath, ETextureBindStage bindStage, UINT slot /*=0u*/)
	: m_Path(filePath), m_Slot(slot), m_BindStage(bindStage)
{
	INFOMAN(gfx);

	// Load in texture from file
	std::wstring wide_path = std::wstring(filePath.begin(), filePath.end());
	wrl::ComPtr<ID3D11Resource> pTexture;
	GFX_THROW_INFO(CreateWICTextureFromFile(GetDevice(gfx), GetContext(gfx), wide_path.c_str(), &pTexture, &pTextureView));

}

void Texture::Bind(Graphics& gfx) noexcept
{
	switch (m_BindStage)
	{
	case ETextureBindStage::Pixel:
		GetContext(gfx)->PSSetShaderResources(m_Slot, 1u, pTextureView.GetAddressOf()); break;
	case ETextureBindStage::Vertex:
		GetContext(gfx)->VSSetShaderResources(m_Slot, 1u, pTextureView.GetAddressOf()); break;
	}
}

std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& path, ETextureBindStage bindStage, UINT slot /*= 0*/)
{
	return Pool::Resolve<Texture>(gfx, path, bindStage, slot);
}

std::string Texture::GenerateUID(const std::string& path, ETextureBindStage bindStage, UINT slot /*= 0*/)
{
	using namespace std::string_literals;
	const std::string stage = bindStage == ETextureBindStage::Vertex ? "vertex" : "pixel";
	return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot) + "#" + stage;
}

std::string Texture::GetUID() const noexcept
{
	return GenerateUID(m_Path, m_BindStage, m_Slot);
}

//////////////////////////////////////////////////////////////////////////
// Sampler
//////////////////////////////////////////////////////////////////////////

Sampler::Sampler(Graphics& gfx, Type type, ETextureBindStage bindStage, UINT slot)
	: m_Type(type), m_Slot(slot), m_BindStage(bindStage)
{
	INFOMAN(gfx);

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

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
	switch (m_BindStage)
	{
	case ETextureBindStage::Pixel:
		GetContext(gfx)->PSSetSamplers(m_Slot, 1u, pSampler.GetAddressOf()); break;
	case ETextureBindStage::Vertex:
		GetContext(gfx)->VSSetSamplers(m_Slot, 1u, pSampler.GetAddressOf()); break;
	}
}

std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx, Type type, ETextureBindStage bindStage, UINT slot)
{
	return Pool::Resolve<Sampler>(gfx, type, bindStage, slot);
}

std::string Sampler::GenerateUID(Type type, ETextureBindStage bindStage, UINT slot)
{
	using namespace std::string_literals;
	const std::string stage = bindStage == ETextureBindStage::Vertex ? "vertex" : "pixel";
	return typeid(Sampler).name() + "#"s + std::to_string((int)type) + "@"s + std::to_string(slot) + stage;
}

std::string Sampler::GetUID() const noexcept
{
	return GenerateUID(m_Type, m_BindStage, m_Slot);
}
