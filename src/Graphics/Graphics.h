#pragma once
#include "RomanceWin.h" // Include first for all our switch cases since d3d11 also includes Windows.h
#include "Errors/DXGIInfoManager.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

// Forward Declaration
namespace RenderResource
{
	class IBindable;
}

/// Rundown of the various parts of D3D11
/// - DEVICE:   Must create a device, acts as an interface between the application and the graphics hardware. We use Device
///             whenever we want to allocate resources like a texture, buffer, shader, etc...
/// - CONTEXT:  We use a context to issue draw commands
class Graphics
{
	friend RenderResource::IBindable;

public:
	Graphics(HWND hWnd);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	/// @brief	Clears our render target view we created from the swapchain on init with a specified color
	void StartFrame() noexcept;
	/// @brief	Presents the back buffer to the front buffer which is the screen (SwapBuffer)
	void EndFrame();

	DirectX::XMMATRIX GetCameraTransform() const noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

	void DrawIndexed(UINT indexCount);
	
	void SetClearColor(float Color[4]) { m_ClearColor[0] = Color[0]; m_ClearColor[1] = Color[1]; m_ClearColor[2] = Color[2]; m_ClearColor[3] = 1; }

private:
	float m_ClearColor[4] = { 0, 0, 0, 1 };

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSView;

	
#ifdef _DEBUG
	DXGIInfoManager m_InfoManager;
#endif
};

