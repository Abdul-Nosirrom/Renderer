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

Shader::Shader(const std::string& inPath)
	: m_Path(inPath)
{}

void Shader::CompileShader(const char* entryPoint, const char* profile)
{
	INFOMAN();

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

PixelShader::PixelShader(const std::string& inPath)
	: Shader(inPath)
{
	INFOMAN();
	CompileShader("PSMain", "ps_5_0");
	GFX_THROW_INFO(GetDevice()->CreatePixelShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::Bind() noexcept
{
	GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

std::shared_ptr<PixelShader> PixelShader::Resolve(const std::string& path)
{
	return BindablePool::Resolve<PixelShader>(path);
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

VertexShader::VertexShader(const std::string& inPath)
	: Shader(inPath)
{
	INFOMAN();
	CompileShader("VSMain", "vs_5_0");
	GFX_THROW_INFO(GetDevice()->CreateVertexShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind() noexcept
{
	GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

std::shared_ptr<VertexShader> VertexShader::Resolve(const std::string& path)
{
	auto pVS = BindablePool::Resolve<VertexShader>(path);
	pLastResolvedByteCode = pVS->GetBytecode();
	return pVS;
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

GeometryShader::GeometryShader(const std::string& inPath)
	: Shader(inPath)
{
	INFOMAN();
	CompileShader("GSMain", "gs_5_0");
	GFX_THROW_INFO(GetDevice()->CreateGeometryShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pGeometryShader));
}

void GeometryShader::Bind() noexcept
{
	GetContext()->GSSetShader(pGeometryShader.Get(), nullptr, 0u);
}

std::shared_ptr<GeometryShader> GeometryShader::Resolve(const std::string& path)
{
	return BindablePool::Resolve<GeometryShader>(path);
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
