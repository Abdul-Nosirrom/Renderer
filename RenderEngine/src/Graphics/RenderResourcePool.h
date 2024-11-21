#pragma once
#include "Core.h"
#include "Bindables/IBindable.h"
#include "Resources/ShaderResource.h"
#include <type_traits>
#include <unordered_map>
#include <memory>
#include <string>

namespace RenderResource
{
	// singleton class for all render resources/bindables (really weird syntax im not used to sorry :( )
	// name this bindable bool, resource pool is different (for ID3D11Resources)
	class RENDERENGINE_API BindablePool
	{
	public:
		// resolve
		template<class T, typename...Params>
		static std::shared_ptr<T> Resolve(Params&&...p)// noexcept
		{
			static_assert(std::is_base_of<IBindable, T>::value, "Can only resolve classes implementing IBindable!");
			return Get().Resolve_Internal<T>(std::forward<Params>(p)...);
		}

		static void ClearRenderTargetViews(const Vector4& clearColor);

		static void ClearDepthStencilViews(float depthVal, float stencilVal);

	private:
		// resolve
		template<class T, typename...Params>
		std::shared_ptr<T> Resolve_Internal(Params&&...p)// noexcept
		{
			const auto key = T::GenerateUID(std::forward<Params>(p)...);
			const auto i = binds.find(key);
			// Doesn't exist in the pool, create it
			if (i == binds.end())
			{
				auto bind = std::make_shared<T>(std::forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}
			// Exists so just return from the pool
			else
			{
				return std::static_pointer_cast<T>(i->second);
			}
		}

		static BindablePool& Get()
		{
			static BindablePool pool;
			return pool;
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<IBindable>> binds;
	};

	// Const special case for the back buffer, it gets auto generated
	const std::string back_buffer = "back_buffer";
	// Resource pool [ID3D11Resource], no need for template fuckery here because we're guarnateed to have 1/2 types max and its straightforward to resolve them
	class RENDERENGINE_API ResourcePool
	{
	public:
		static ResourcePool& Get()
		{
			static ResourcePool pool;
			return pool;
		}


		// NOTE: For now forget about loading from file, just focus on generting render targets for render passes, we'll re-add texture reading later as maybe a more explicit subclass of SHaderResource [Texture2DResource]

		static std::shared_ptr<ShaderResource> Resolve(const std::string& name, const TEXTURE_DESC& desc)
		{
			return Get().Resolve_Internal(name, desc);
		}

	private:
		std::shared_ptr<ShaderResource> Resolve_Internal(const std::string& name, const TEXTURE_DESC& desc)
		{
			const std::string key = name;//ShaderResource::GetUID(name, desc);
			const auto i = m_resources.find(key);
			// Doesn't exist in the pool, create it
			if (i == m_resources.end())
			{
				auto bind = std::make_shared<ShaderResource>(name, desc);
				m_resources[key] = bind;
				return bind;
			}
			// Exists so just return from the pool
			else
			{
				return i->second;
			}
		}

		std::unordered_map<std::string, std::shared_ptr<ShaderResource>> m_resources;
	};
}