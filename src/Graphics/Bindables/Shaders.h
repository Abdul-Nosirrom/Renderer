#pragma once
#include <memory>

#include "IBindable.h"

namespace RenderResource
{
	/// @brief	Base interface class for shader bindables
	class Shader : public IBindable
	{
	public:
		Shader(Graphics& gfx, const std::string& inPath);
		ID3DBlob* GetBytecode() const noexcept { return pShaderBytecode.Get(); }
		
	protected:
		void CompileShader(Graphics& gfx, const char* entryPoint, const char* profile);

	protected:
		std::string m_Path;
		Microsoft::WRL::ComPtr<ID3DBlob> pShaderBytecode;
	};

	class PixelShader : public Shader
	{
	public:
		PixelShader(Graphics& gfx, const std::string& inPath);
		virtual void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const noexcept override;
	
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader(Graphics& gfx, const std::string& inPath);
		virtual void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<VertexShader> Resolve(Graphics& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const noexcept override;
	
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};

	class GeometryShader : public Shader
	{
	public:
		GeometryShader(Graphics& gfx, const std::string& inPath);
		virtual void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<GeometryShader> Resolve(Graphics& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const noexcept override;
	
	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> pGeometryShader;
	};
}
