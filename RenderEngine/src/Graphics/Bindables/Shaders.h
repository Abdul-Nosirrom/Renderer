#pragma once
#include <memory>
#include "IBindable.h"

namespace RenderResource
{
	/// @brief	Base interface class for shader bindables
	class Shader : public IBindable
	{
	public:
		Shader(const std::string& inPath);
		ID3DBlob* GetBytecode() const noexcept { return pShaderBytecode.Get(); }
		
	protected:
		void CompileShader(const char* entryPoint, const char* profile);

	protected:
		std::string m_Path;
		Microsoft::WRL::ComPtr<ID3DBlob> pShaderBytecode;
	};

	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::string& inPath);
		virtual void Bind() noexcept override;
		static std::shared_ptr<PixelShader> Resolve(const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const noexcept override;
	
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::string& inPath);
		virtual void Bind() noexcept override;
		static std::shared_ptr<VertexShader> Resolve(const std::string& path);
		static ID3DBlob* GetLastByteCode() { return pLastResolvedByteCode; }
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const noexcept override;
	
	private:
		// Byte code of the last resolved vertex shader, just to not need to pass references along (tho this introduces a strong condition on order of bindings thats easier to miss but its ok)
		// Can have the input layout bind null this, and if we try to receive it while its null it eats shit (exception)
		inline static ID3DBlob* pLastResolvedByteCode = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};

	class GeometryShader : public Shader
	{
	public:
		GeometryShader(const std::string& inPath);
		virtual void Bind() noexcept override;
		static std::shared_ptr<GeometryShader> Resolve(const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const noexcept override;
	
	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> pGeometryShader;
	};
}
