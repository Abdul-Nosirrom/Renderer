#pragma once
#include "Bindables/IBindable.h"
#include <type_traits>
#include <unordered_map>
#include <memory>
#include <string>

namespace RenderResource
{
	// singleton class for all render resources/bindables (really weird syntax im not used to sorry :( )
	class Pool
	{
	public:
		// resolve
		template<class T, typename...Params>
		static std::shared_ptr<T> Resolve(Graphics& gfx, Params&&...p)// noexcept
		{
			static_assert(std::is_base_of<IBindable, T>::value, "Can only resolve classes implementing IBindable!");
			return Get().Resolve_Internal<T>(gfx, std::forward<Params>(p)...);
		}
	private:
		// resolve
		template<class T, typename...Params>
		std::shared_ptr<T> Resolve_Internal(Graphics& gfx, Params&&...p)// noexcept
		{
			const auto key = T::GenerateUID(std::forward<Params>(p)...);
			const auto i = binds.find(key);
			// Doesn't exist in the pool, create it
			if (i == binds.end())
			{
				auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}
			// Exists so just return from the pool
			else
			{
				return std::static_pointer_cast<T>(i->second);
			}
		}

		static Pool& Get()
		{
			static Pool pool;
			return pool;
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<IBindable>> binds;
	};
}