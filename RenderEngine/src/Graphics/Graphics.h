#pragma once
#include "Core.h"
#include "Errors/DXGIInfoManager.h"
#include <d3d11.h>
#include <d3d11_1.h>

#include "MathCore.h"
#include <wrl.h>

// Forward Declaration
namespace RenderResource
{
	class IBindable;
}
class Window;

/// Rundown of the various parts of D3D11
/// - DEVICE:   Must create a device, acts as an interface between the application and the graphics hardware. We use Device
///             whenever we want to allocate resources like a texture, buffer, shader, etc...
/// - CONTEXT:  We use a context to issue draw commands
class RENDERENGINE_API Graphics
{
	friend RenderResource::IBindable;
	friend Window;

public:
	Graphics(HWND hWnd);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	/// @brief	Clears our render target view we created from the swapchain on init with a specified color
	void StartFrame() noexcept;
	/// @brief	Presents the back buffer to the front buffer which is the screen (SwapBuffer)
	void EndFrame();

	Matrix4x4 GetProjection() const noexcept;

	void DrawIndexed(UINT indexCount);
	
	void SetClearColor(const Vector4& Color) noexcept { m_ClearColor = Color; }

	void ResizeWindow(const Vector2& NewSize);

	// Debug
	void StartEvent(const std::wstring& name) const noexcept
	{
		pDebugAnnotation->BeginEvent(name.c_str());
	}
	void EndEvent() const noexcept
	{
		pDebugAnnotation->EndEvent();
	}
	//~

private:

	void CreateRenderTargets();
	void ReleaseRenderTargets();
	void SetupViewport();

	Vector4 m_ClearColor = Vector4(0,0,0,1);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> pDebugAnnotation;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSView;

	Vector2 ViewportSize;
	
#ifdef _DEBUG
	DXGIInfoManager m_InfoManager;
#endif
};

