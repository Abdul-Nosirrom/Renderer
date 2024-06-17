#include "Shaders.h"
#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"
#include <d3dcompiler.h>

using namespace RenderResource;
namespace wrl = Microsoft::WRL;

#pragma comment(lib, "d3dcompiler.lib")

//////////////////////////////////////////////////////////////////////////
// IShader Interface
//////////////////////////////////////////////////////////////////////////

Shader::Shader(Graphics& gfx, const std::string& inPath) 
	: m_Path(inPath)
{}

void Shader::CompileShader(Graphics& gfx, const char* entryPoint, const char* profile)
{
	INFOMAN(gfx); 

	wrl::ComPtr<ID3DBlob> pErrorBlob;
	std::wstring shaderpath = std::wstring(m_Path.begin(), m_Path.end());

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif 

	// Cant use throw macro, have to retrieve error msg from pErrorBlob
	HRESULT compHR = D3DCompileFromFile(
		shaderpath.c_str(),
		nullptr,
		nullptr,
		entryPoint,
		profile,
		flags, 0u, &pShaderBytecode, &pErrorBlob);

	if (compHR < 0)
	{
		if (pErrorBlob != nullptr)
		{
			auto err = (char*)pErrorBlob->GetBufferPointer();
			throw GFXException(__LINE__, __FILE__, compHR, { err });
		}
		else
		{
			GFX_THROW_INFO(compHR);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Pixel Shader
//////////////////////////////////////////////////////////////////////////

PixelShader::PixelShader(Graphics& gfx, const std::string& inPath)
	: Shader(gfx, inPath)
{
	INFOMAN(gfx);
	CompileShader(gfx, "PSMain", "ps_5_0");
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::string& path)
{
	return Pool::Resolve<PixelShader>(gfx, path);
}

std::string PixelShader::GenerateUID(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(PixelShader).name() + "#"s + path;
}

std::string PixelShader::GetUID() const noexcept
{
    return GenerateUID(m_Path);
}

//////////////////////////////////////////////////////////////////////////
// Vertex Shader
//////////////////////////////////////////////////////////////////////////

VertexShader::VertexShader(Graphics& gfx, const std::string& inPath)
	: Shader(gfx, inPath)
{
	INFOMAN(gfx);
	CompileShader(gfx, "VSMain", "vs_5_0");
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& path)
{
	return Pool::Resolve<VertexShader>(gfx, path);
}

std::string VertexShader::GenerateUID(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + path;
}

std::string VertexShader::GetUID() const noexcept
{
	return GenerateUID(m_Path);
}

//////////////////////////////////////////////////////////////////////////
// Geometry Shader
//////////////////////////////////////////////////////////////////////////

GeometryShader::GeometryShader(Graphics& gfx, const std::string& inPath)
	: Shader(gfx, inPath)
{
	INFOMAN(gfx);
	CompileShader(gfx, "GSMain", "gs_5_0");
	GFX_THROW_INFO(GetDevice(gfx)->CreateGeometryShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pGeometryShader));
}

void GeometryShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->GSSetShader(pGeometryShader.Get(), nullptr, 0u);
}

std::shared_ptr<GeometryShader> GeometryShader::Resolve(Graphics& gfx, const std::string& path)
{
	return Pool::Resolve<GeometryShader>(gfx, path);
}

std::string GeometryShader::GenerateUID(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(GeometryShader).name() + "#"s + path;
}

std::string GeometryShader::GetUID() const noexcept
{
	return GenerateUID(m_Path);
}
