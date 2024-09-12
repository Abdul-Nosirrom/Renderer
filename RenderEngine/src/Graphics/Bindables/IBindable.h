#pragma once
#include "Graphics/Graphics.h"
#include <cassert>

namespace RenderResource
{
	/// @brief	Interface for all render bindables (pixel shaders, textures, etc...)
	class IBindable
	{
	public:
		virtual void Bind(Graphics& gfx) noexcept = 0;
		virtual std::string GetUID() const noexcept
		{
			assert(false);
			return "";
		}
		virtual ~IBindable() = default;

	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept { return gfx.pContext.Get();  }
		static ID3D11Device* GetDevice(Graphics& gfx) noexcept { return gfx.pDevice.Get();  }
		static DXGIInfoManager& GetInfoManager(Graphics& gfx)
		{
#ifdef _DEBUG 
			return gfx.m_InfoManager;
#else 
			throw std::logic_error("Error! Tried accessing Gfx.InfoManager in Release Config!");
#endif 
		}
	};
}

