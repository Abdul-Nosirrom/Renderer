#pragma once
#include <utility>

#include "Core.h"
#include "Graphics/Renderer.h"
#include "Errors/WindowExceptions.h"

namespace RenderResource
{
	using TEX_BIND_FLAGS = int;
	struct TEXTURE_DESC
	{
		DXGI_FORMAT Format;
		TEX_BIND_FLAGS BindFlags;
	};

	class RENDERENGINE_API ShaderResource
	{
	public:
		// we store the bind flags because different bind flags = different resource. In the resource view, each of the 3 types expects a certain bind flag so we also use it to ensure shits ok there
		//ShaderResource(const std::string& filePath);
		ShaderResource(const std::string& name, const TEXTURE_DESC& desc);
		virtual ~ShaderResource() = default;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D() const
		{
			return pResource;
		}

		TEX_BIND_FLAGS BindFlags() const noexcept { return m_texDesc.BindFlags; }

		std::string GetUID() const noexcept { return m_name; }

	protected:

		// template<typename T>
		// Microsoft::WRL::ComPtr<T> Get() const
		// {
		// 	Microsoft::WRL::ComPtr<T> resource;
		// 	HRESULT hr = pResource.As(&resource);
		//
		// 	if (FAILED(hr)) throw RDWND_EXCEPT(hr);
		//
		// 	return resource;
		// }

		static DXGIInfoManager& GetInfoManager()
		{
#ifdef _DEBUG
			return Renderer::m_InfoManager;
#else
			throw std::logic_error("Error! Tried accessing Gfx.InfoManager in Release Config!");
#endif
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pResource;
		TEXTURE_DESC m_texDesc;
		std::string m_name;
	};
}
