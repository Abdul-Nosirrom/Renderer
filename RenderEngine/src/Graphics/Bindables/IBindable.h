#pragma once
#include "Graphics/Renderer.h"
#include <cassert>

namespace RenderResource
{
	/// @brief	Interface for all render bindables (pixel shaders, textures, etc...)
	class IBindable
	{
	public:
		virtual void Bind() noexcept = 0;
		virtual std::string GetUID() const noexcept
		{
			assert(false);
			return "";
		}
		virtual ~IBindable() = default;

	protected:
		static ID3D11DeviceContext* GetContext() noexcept { return Renderer::GetContext();  }
		static ID3D11Device* GetDevice() noexcept { return Renderer::GetDevice();  }
		static DXGIInfoManager& GetInfoManager()
		{
#ifdef _DEBUG 
			return Renderer::m_InfoManager;
#else 
			throw std::logic_error("Error! Tried accessing Gfx.InfoManager in Release Config!");
#endif 
		}
	};
}

