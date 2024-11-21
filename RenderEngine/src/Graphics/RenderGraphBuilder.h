#pragma once
#include "Core.h"
#include <d3d11.h>
#include <functional>
#include <string>
#include <unordered_map>

#include "Renderer.h"

/*
enum class ERenderPassIO
{
	SRV,
	RenderTarget,
	DepthStencil
};

class RenderGraphBuilder
{
public:
	static void GetPassIO(const std::string& resourceName, ERenderPassIO type);

	void CreateTexture(const std::string& name, D3D11_TEXTURE2D_DESC& desc);

	
	void CreateRenderTarget(const std::string& name, D3D11_RENDER_TARGET_VIEW_DESC& desc);

private:
	std::unordered_map<std::string, class ID3D11Resource*> passes;
};
*/

using RenderPassFunction = std::function<void()>;
struct RENDERENGINE_API RenderPassInfo
{
	std::wstring name;
	RenderPassFunction renderPass;
};

class RENDERENGINE_API RenderGraph
{
public:
	static void AddPass(const std::wstring& name, const RenderPassFunction& function)
	{
		m_passes.emplace_back(name, function);
	}
	static void Execute()
	{
		for (auto& pass : m_passes)
		{
			RENDER_EVENT(RDGPass, pass.name);
			pass.renderPass();
		}
	}

private:
	inline static std::vector<RenderPassInfo> m_passes = std::vector<RenderPassInfo>();
};

