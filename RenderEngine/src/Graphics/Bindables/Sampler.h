#pragma once
#include "IBindable.h"
#include <memory>

#include "Graphics/Resources/Views/ResourceView.h"

namespace RenderResource 
{
	class Sampler : public IBindable
	{
	public:
		enum class Type
		{
			Anisotropic,
			Bilinear,
			Point,
		};
	public:
		Sampler(Type type, ESRVBindStage bindStage, UINT slot = 0u);
		void Bind() noexcept override;
		static std::shared_ptr<Sampler> Resolve(Type type, ESRVBindStage bindStage, UINT slot = 0u);
		static std::string GenerateUID(Type type, ESRVBindStage bindStage, UINT slot);
		std::string GetUID() const noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		Type m_Type;
		UINT m_Slot;
		ESRVBindStage m_BindStage;
	};
}
