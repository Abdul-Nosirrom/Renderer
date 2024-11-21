#pragma once
#include "Core.h"
#include "Errors/DXGIInfoManager.h"
#include <d3d11.h>
#include <d3d11_1.h>
#include "MathCore.h"
#include <wrl.h>

namespace RenderResource
{
	class IBindable;
	class ShaderResource;
}

/*
 * Renderer: In charge of dispatching draws and serves as the main place to create and dispatch
*  render passes
*   - Manages the render graph
*   -
*
 * RHI: In charge of allocating and compiling resources
*
 * GraphBuilder: Object stored by renderer that stores the render pass functors
*     - It also manages graph resources to ensure proper execution order
 *
 * We don't need any significant resource management, just executing in the seq passes were added is well and good!
 *
 * */
class RENDERENGINE_API Renderer
{
	friend RenderResource::IBindable;
	friend RenderResource::ShaderResource;
	friend class Window;
	friend struct FScopedRenderEvent;

public:

	static ID3D11Device* GetDevice() { return pDevice.Get(); }
	static ID3D11DeviceContext* GetContext() { return pContext.Get(); }

	static bool Init(HWND hwnd);
	static void Shutdown();

	/// @brief	Clears our render target view we created from the swapchain on init with a specified color
	static void StartFrame() noexcept;
	static void Draw(UINT indexCount);
	/// @brief	Presents the back buffer to the front buffer which is the screen (SwapBuffer)
	static void EndFrame();

	static Matrix4x4 GetProjectionMatrix() noexcept;

	static Vector2 GetViewportSize() noexcept { return m_ViewportSize; }

	enum class EViewportSize
	{
		QUARTER, HALF, FULL
	};

	static void SetRenderViewportSize(EViewportSize size);

	static ID3D11RenderTargetView* GetBackBuffer() noexcept { return pBackBufferRTV.Get(); }

protected:

	static void ResizeWindow(const Vector2& NewSize);

	static void CreateRenderTargets();
	static void ReleaseRenderTargets();
	static void SetupViewport();

	inline static Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	inline static Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	inline static Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> pDebugAnnotation = nullptr;
	inline static Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;

	// Const resources
	inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pBackBufferRTV = nullptr;
	//inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget = nullptr;
	//inline static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSView = nullptr;

#ifdef _DEBUG
	inline static DXGIInfoManager m_InfoManager;
#endif

	inline static Vector2 m_ViewportSize = Vector2(800,800);
};

#define RENDER_EVENT(event, name) FScopedRenderEvent RenderEvent_##event##(name);

struct RENDERENGINE_API FScopedRenderEvent
{
	FScopedRenderEvent(const std::wstring& eventName)
	{
		Renderer::pDebugAnnotation->BeginEvent(eventName.c_str());
	}
	~FScopedRenderEvent()
	{
		Renderer::pDebugAnnotation->EndEvent();
	}
};


class RenderPassBase
{
public:
	virtual void SetupPass(class GraphBuilder &graphBuilder) = 0;

	virtual void ExecutePass(GraphBuilder &graphBuilder) = 0;
};

class GraphBuilder
{
public:
	void Execute()
	{
		// execute render features which will subsequently execute passes
		// Resources are managed by the graph builder, and that's so we can share resources between passes, lets maybe use a string
		// e.g one pass outputs its render target to be read by the next pass
		// These resources i think are only meant in the case of textures, specifically non-readonly texs
	}

	static void AddPass(const RenderPassBase& Pass) {}

	std::vector<class RenderFeature *> m_RenderFeature;
};


template<typename ParamStruct>
class RenderPass : public RenderPassBase
{
public:
	RenderPass(std::function<void(ParamStruct data, GraphBuilder &graphBuilder)> setupFunctor,
		std::function<void(const ParamStruct &data)> execFunctor) : setup_(std::move(setupFunctor)), execute_(std::move(execFunctor)) {}

	void SetupPass(GraphBuilder &graphBuilder) override
	{
		setup_(passData, graphBuilder);
	}

	void ExecutePass(GraphBuilder &graphBuilder) override
	{
		execute_(passData);
	}

protected:
	ParamStruct passData;
	std::function<void(ParamStruct data, GraphBuilder &graphBuilder)> setup_;
	std::function<void(const ParamStruct &data)> execute_;

	std::vector<ID3D11Resource *> m_passInputs;
	std::vector<ID3D11Resource *> m_passOutputs;
};


/*

// Holds a collection of render passes [e.g blo-m has a threshold, downsample CS, blur X, blur Y, upsample CS]
// Can also add "Named Events" to graph builder functions to specify render event name [Feature -> pass names] is nice
class RenderFeature
{
public:
	void Execute(GraphBuilder &graphBuilder)
	{
		for (auto &pass: m_RenderPasses)
		{
			pass->SetupPass(graphBuilder);
			pass->ExecutePass(graphBuilder);
		}
	}

	void AddRenderPass(RenderPassBase *Inpass);

	std::vector<RenderPassBase *> m_RenderPasses;
};
*/