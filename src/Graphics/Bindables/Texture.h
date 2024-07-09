#pragma once
#include "IBindable.h"
#include <memory>

namespace RenderResource 
{
	enum class ETextureBindStage : UINT
	{
		Pixel,
		Vertex
	};

	class Texture : public IBindable
	{
	public:
		Texture(Graphics& gfx, const std::string& filePath, ETextureBindStage bindStage, UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, ETextureBindStage bindStage, UINT slot = 0u);
		static std::string GenerateUID(const std::string& path, ETextureBindStage bindStage, UINT slot = 0);

		std::string GetUID() const noexcept override;

	protected:
		UINT m_Slot;
		ETextureBindStage m_BindStage;

		std::string m_Path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};

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
		Sampler(Graphics& gfx, Type type, ETextureBindStage bindStage, UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Sampler> Resolve(Graphics& gfx, Type type, ETextureBindStage bindStage, UINT slot = 0u);
		static std::string GenerateUID(Type type, ETextureBindStage bindStage, UINT slot);
		std::string GetUID() const noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		Type m_Type;
		UINT m_Slot;
		ETextureBindStage m_BindStage;
	};
}
