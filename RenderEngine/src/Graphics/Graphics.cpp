#include "Graphics.h"
#include "Errors/GraphicsExceptions.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"

// namespace for our com ptrs
namespace wrl = Microsoft::WRL;

// Specify linking to d3d11 library (here instead of project settings)
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")


Graphics::Graphics(HWND hWnd)
{
	ViewportSize = {800, 800};
	// Set up configuration struct for our swap chain
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;                                                /* Buffer Width, 0 to default to window */
	sd.BufferDesc.Height = 0;                                               /* Buffer Height, 0 to default to window */
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						/* Buffer Format, pixel layout (RGBA, 16) */
	sd.BufferDesc.RefreshRate.Numerator = 0;                                /* Default refresh rate values for buffer front/back swap */
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                  /* Scaling method, none since we default to the window */
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;  /* Scaline order, useful if display is interlaced */
	sd.SampleDesc.Count = 1;                                                /* AntiAliasing, 1 sample with 0 quality meaning no AA */
	sd.SampleDesc.Quality = 0;                                              /* AA quality 0, disabled */
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                       /* Buffer usage, to render target output */
	sd.BufferCount = 2;                                                     /* = 1 means double buffering, front buffer and back buffer (set to 2 because using Flip for swap effect and it said i had to) */
	sd.OutputWindow = hWnd;                                                 /* Window handle to draw on */
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                          /* How to handle front buffer after presenting a surface, we specify to discard it after */
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;

#ifdef _DEBUG 
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG; // Create debug layer to be able to retrieve debug messages associated with HResults
#endif

	// Create the device now and swap chain
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,                    /* Graphics adapter, setting to null lets it choose it for us */
		D3D_DRIVER_TYPE_HARDWARE,   /* Driver type, whether software acceleration or hardware. Hardware for us */
		nullptr,                    /* Must be null if DriverType is not software (its hardware for us). Handle to binary to our graphics driver to use */
		swapCreateFlags,            /* Various creation flags, here we specify to create the device on the debug layer so we can retrieve detailed debug outputs */
		nullptr,                    /* Feature level to support (D3D9, 10, 11, etc..), null results in a default feature level (see docs) */
		0,                          /* Number of feature levels being used, guessing if the above is null, it'll auto this */
		D3D11_SDK_VERSION,          /* SDK Version, self explanatory */
		&sd,                        /* Configuration struct for our swap chain */
		&pSwapChain,                /* Pointer to our swap chain, to be filled out upon creation */
		&pDevice,                   /* Pointer to our ID3D11 device, to be filled out upon creation */
		nullptr,                    /* Out Pointer, would fill out with the feature level actually secured */
		&pContext                   /* Pointer to our device context, to be filled out upon creation */
	));

	CreateRenderTargets();
	SetupViewport();

	// Get the Annotations interface (Used for specifying named events with graphics command blocks to be viewed in RenderDoc)
	GFX_THROW_INFO(pContext->QueryInterface(IID_PPV_ARGS(&pDebugAnnotation)));
}

void Graphics::StartFrame() noexcept
{
#ifdef _DEBUG
	m_InfoManager.Set(); // To only get the latest messages, essentially clearing previous frames msg history
#endif

	// Clear the back buffer
	pContext->ClearRenderTargetView(pRenderTarget.Get(), m_ClearColor.data());
	// Clear depth buffer
	pContext->ClearDepthStencilView(pDSView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
}

void Graphics::EndFrame()
{
	GFX_DEVICE_REMOVED_EXCEPT(pSwapChain->Present(1, 0u));

	// Bind for next frame as it gets unbound during Present when swap effect is set to DXGI_SWAP_EFFECT_FLIP_DISCARD
	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSView.Get());

}

Matrix4x4 Graphics::GetProjection() const noexcept
{
	return MatrixConstruction::PerspectiveMatrix(Scene::Get().GetViewFoV(), ViewportSize.y()/ViewportSize.x(), 0.5f, 100.f); // aspect ratio, znear, zfar
}

void Graphics::DrawIndexed(UINT indexCount)
{
	// Temp wire-frame debug
	{
		wrl::ComPtr<ID3D11RasterizerState> pRSS;
		D3D11_RASTERIZER_DESC rsd = {};
		rsd.FillMode = D3D11_FILL_WIREFRAME;
		rsd.CullMode = D3D11_CULL_NONE;//D3D11_CULL_BACK;
		
		rsd.FrontCounterClockwise = FALSE;

		GFX_THROW_INFO(pDevice->CreateRasterizerState(&rsd, &pRSS));
		pContext->RSSetState(pRSS.Get());
	}
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(indexCount, 0u, 0u));
}

void Graphics::ResizeWindow(const Vector2 &NewSize)
{
	ViewportSize = NewSize;

	if (pSwapChain)
	{
		ReleaseRenderTargets();

		// Buffer Count & Format & Flags: By passing in 0 here & FORMAT_UNKNOWN, what that essentially does is preserve the old buffcount/format/flags upon creation
		// 0s for size means use the rect size of the hwnd that was passed on initialize
		GFX_THROW_INFO(pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

		CreateRenderTargets();

		SetupViewport();
	}
}

void Graphics::CreateRenderTargets()
{
	// Gain access to texture subresource in swap chain (back buffer)
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	// Similar to QueryInterface from COM in terms of inputs, except we're querying a resource on the interface
	GFX_THROW_INFO(pSwapChain->GetBuffer(
		0,                             /* Index of buffer we wanna get, 0 gives us the back buffer*/
		UUID(ID3D11Resource),          /* UUID thing of COM interface, a D3D11 resource in our case */
		(&pBackBuffer)                 /* PP to our resource to be filled out */
	));

	assert(pDevice.Get() && pContext.Get() && pBackBuffer.Get());
	//assert(pContext.Get());

	// Create a reference to the render target view to access and modify later. This is getting a reference to the back buffer, so we're drawing to that render target
	// which is the back buffer, then upon swapping buffer presenting it (in which the front buffer now becomes the back buffer we draw to next frame)
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),      /* Buffer in which to receive the render target from */
		nullptr,                /* Config struct to specify how we wanna receive the RTV, default it */
		&pRenderTarget          /* ID3D11 Target to be filled out */
	));

	//////////////////////////////////////////////////////////////////////////
	// Create Depth Stencil State/View & Bind for 3D Rendering Depth Tests
	//////////////////////////////////////////////////////////////////////////

	// Define the state of the depth stencil obj, we're only enabling depth tests here not stencil tests and setting to to compare by 'less' op
	// Essentially describing how our depth tests are gonna work
	D3D11_DEPTH_STENCIL_DESC dssDesc = {};
	dssDesc.DepthEnable = TRUE;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS;

	wrl::ComPtr<ID3D11DepthStencilState> pDSState;

	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dssDesc, &pDSState));

	pContext->OMSetDepthStencilState(pDSState.Get(), 0u); // bind depth state to object merger

	// Create depth texture, this is where depth results will go to
	wrl::ComPtr<ID3D11Texture2D> pDSTexture;
	D3D11_TEXTURE2D_DESC dstDesc = {};
	dstDesc.Width = ViewportSize.x(); // Default to rect size
	dstDesc.Height = ViewportSize.y();
	dstDesc.MipLevels = 1u; // disable mips for depth texture
	dstDesc.ArraySize = 1u; // not an array of textures
	dstDesc.Format = DXGI_FORMAT_D32_FLOAT; // depth texture with no stencil is just D32
	dstDesc.SampleDesc.Count = 1u; // no AA
	dstDesc.SampleDesc.Quality = 0u;
	dstDesc.Usage = D3D11_USAGE_DEFAULT;
	dstDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // Important to specify this for this texture

	GFX_THROW_INFO(pDevice->CreateTexture2D(&dstDesc, nullptr, &pDSTexture)); // No initial data because it'll be filled for us each frame

	// In order to bind a texture to the pipeline, need to create a 'view' for that texture
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0u;

	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, &pDSView));


	// Finally bind the depth stencil view to the object merger
	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSView.Get());
}

void Graphics::ReleaseRenderTargets()
{
	// Unbind render target
	pContext->OMSetRenderTargets(0, 0, 0);

	// Release current render target & DSV, we'll create a new one with the correct dimensions
	pRenderTarget = nullptr; // Calling release makes it crash idk why
	pDSView = nullptr;

}

void Graphics::SetupViewport()
{
	// Setup our viewport
	D3D11_VIEWPORT viewPort = {};
	viewPort.Width = ViewportSize.x();
	viewPort.Height = ViewportSize.y();
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pContext->RSSetViewports(1u, &viewPort);
}
