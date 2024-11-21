#include "ShaderResource.h"
#include "Errors/GraphicsExceptions.h"
#include "Utility/WICTextureLoader.h"


/*
ShaderResource::ShaderResource(const std::string &filePath)
{
	INFOMAN();

	// Load in texture from file
	std::wstring wide_path = std::wstring(filePath.begin(), filePath.end());
	wrl::ComPtr<ID3D11Resource> pTexture;
	GFX_THROW_INFO(CreateWICTextureFromFile(Renderer::GetDevice(), Renderer::GetContext(), wide_path.c_str(), &pTexture, nullptr));

}
*/

namespace wrl = Microsoft::WRL;
using namespace RenderResource;

RenderResource::ShaderResource::ShaderResource(const std::string& name, const TEXTURE_DESC &desc)
	: m_texDesc(desc), m_name(name)
{
	INFOMAN();

	// Construct description
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.BindFlags = m_texDesc.BindFlags;
	texDesc.Format = m_texDesc.Format;
	texDesc.Width = static_cast<UINT>(Renderer::GetViewportSize().x());
	texDesc.Height = static_cast<UINT>(Renderer::GetViewportSize().y());
	texDesc.MipLevels = 1u; // disable mips for depth texture
	texDesc.ArraySize = 1u; // not an array of textures
	texDesc.SampleDesc.Count = 1u; // no AA
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	//texDesc.BindFlags = m_texDesc.BindFlags;
	// height widt & how to handle viewport update

	GFX_THROW_INFO(Renderer::GetDevice()->CreateTexture2D(&texDesc, nullptr, &pResource));
}
