#include "Graphics.h"
#include "Errors/GraphicsExceptions.h"

// namespace for our com ptrs
namespace wrl = Microsoft::WRL;

// Specify linking to d3d11 library (here instead of project settings)
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DXGI.lib")


Graphics::Graphics(HWND hWnd)
{
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

	// Gain access to texture subresource in swap chain (back buffer)
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	// Similar to QueryInterface from COM in terms of inputs, except we're querying a resource on the interface
	GFX_THROW_INFO(pSwapChain->GetBuffer(
		0,                             /* Index of buffer we wanna get, 0 gives us the back buffer*/
		UUID(ID3D11Resource),          /* UUID thing of COM interface, a D3D11 resource in our case */
		(&pBackBuffer)                 /* PP to our resource to be filled out */
	));

	// Create a reference to the render target view to access and modify later. This is getting a reference to the back buffer, so we're drawing to that render target
	// which is the back buffer, then upon swapping buffer presenting it (in which the front buffer now becomes the back buffer we draw to next frame)
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),      /* Buffer in which to receive the render target from */
		nullptr,                /* Config struct to specify how we wanna receive the RTV, default it */
		&pRenderTarget          /* ID3D11 Target to be filled out */
	));

	// Setup our viewport
	D3D11_VIEWPORT viewPort = {};
	viewPort.Width = 800;
	viewPort.Height = 600;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pContext->RSSetViewports(1u, &viewPort);
}

void Graphics::StartFrame() noexcept
{
#ifdef _DEBUG
	m_InfoManager.Set(); // To only get the latest messages, essentially clearing previous frames msg history
#endif

	// Clear the back buffer
	pContext->ClearRenderTargetView(pRenderTarget.Get(), m_ClearColor);
}

void Graphics::EndFrame()
{
	GFX_DEVICE_REMOVED_EXCEPT(pSwapChain->Present(1, 0u));
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Drawing Tests                                                         
/************************************************************************/
//////////////////////////////////////////////////////////////////////////

#include <d3dcompiler.h>
#include <chrono>

void Graphics::Draw()
{
	//////////////////////////////////////////////////////////////////////////
	// VERTEX & INDEX BUFFER SETUP
	//////////////////////////////////////////////////////////////////////////
	struct vertex
	{
		float x, y;
		float r, g, b;
	};

	constexpr vertex tri[3] = {
		{-0.5, -0.5, 
			0.5f, 0.f, 0.f},
		{0, 0.5,
			0.f, 0.5f, 0.f},
		{0.5, -0.5,
			0.f, 0.f, 0.5f}
	};
	UINT stride = sizeof(vertex);
	UINT offset = 0u;

	constexpr UINT idxData[3] = { 2, 1, 0 };

	D3D11_BUFFER_DESC vbd;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = UINT(sizeof(vertex) * 3);
	vbd.StructureByteStride = sizeof(vertex);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = tri;

	wrl::ComPtr<ID3D11Buffer> pBuffer;
	
	GFX_THROW_INFO(pDevice->CreateBuffer(&vbd, &data, pBuffer.GetAddressOf()));

	pContext->IASetVertexBuffers(0u, 1u, pBuffer.GetAddressOf(), &stride, &offset);

	// Set index buffer
	D3D11_BUFFER_DESC idb;
	idb.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idb.Usage = D3D11_USAGE_DEFAULT;
	idb.CPUAccessFlags = 0u;
	idb.MiscFlags = 0u;
	idb.ByteWidth = UINT(sizeof(UINT) * 3);
	idb.StructureByteStride = sizeof(UINT);

	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = idxData;

	wrl::ComPtr<ID3D11Buffer> pIBuffer;
	GFX_THROW_INFO(pDevice->CreateBuffer(&idb, &idata, pIBuffer.GetAddressOf()));
	pContext->IASetIndexBuffer(pIBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//////////////////////////////////////////////////////////////////////////
	// SHADER COMPILATION AND SETUP
	//////////////////////////////////////////////////////////////////////////
	
	// Shader
	auto CompileShader = [&](const std::wstring& path, const char* entryPoint, const char* profile, ID3DBlob** ppBlob)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob;

		UINT flags = 0u; //D3DCOMPILE_ENABLE_STRICTNESS;

		// Cant use throw macro, have to retrieve error msg from pErrorBlob
		HRESULT compHR = D3DCompileFromFile(
			path.c_str(),
			nullptr,
			nullptr,
			entryPoint,
			profile,
			flags, 0u, ppBlob, &pErrorBlob);

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
	};

	
	// Compile shaders
	
	wrl::ComPtr<ID3DBlob> pVSBytecodeBlob;
	wrl::ComPtr<ID3DBlob> pGSBytecodeBlob;
	wrl::ComPtr<ID3DBlob> pPSBytecodeBlob;

	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	wrl::ComPtr<ID3D11GeometryShader> pGeoShader;
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;


	CompileShader(L"shaders/Shaders.hlsl", "VSMain", "vs_5_0", &pVSBytecodeBlob);
	CompileShader(L"shaders/Shaders.hlsl", "GSMain", "gs_5_0", &pGSBytecodeBlob);
	CompileShader(L"shaders/Shaders.hlsl", "PSMain", "ps_5_0", &pPSBytecodeBlob);


	// Create shader from compiled bytecode
	GFX_THROW_INFO(pDevice->CreateVertexShader(pVSBytecodeBlob->GetBufferPointer(), pVSBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader));
	GFX_THROW_INFO(pDevice->CreateGeometryShader(pGSBytecodeBlob->GetBufferPointer(), pGSBytecodeBlob->GetBufferSize(), nullptr, &pGeoShader));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pPSBytecodeBlob->GetBufferPointer(), pPSBytecodeBlob->GetBufferSize(), nullptr, &pPixelShader));

	// Set the shaders
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pContext->GSSetShader(pGeoShader.Get(), nullptr, 0u);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//////////////////////////////////////////////////////////////////////////
	// Update Constant buffer
	//////////////////////////////////////////////////////////////////////////
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;
	static auto t0 = Time::now();
	auto t1 = Time::now();
	fsec tInS = t1 - t0;

	typedef struct cbufferEx
	{
		float timeVal;
		float padding[3];
	} timeData;

	timeData t;
	t.timeVal = tInS.count();
	t.padding[0] = 0.f; t.padding[1] = 1.f; t.padding[2] = 0.5f;


	// If we don't have a CBuffer, create it, otherwise update it
	if (!pConstantBuffer)
	{
		D3D11_BUFFER_DESC cbuf = { 0 };
		cbuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbuf.Usage = D3D11_USAGE_DYNAMIC;
		cbuf.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbuf.MiscFlags = 0u;
		cbuf.ByteWidth = sizeof(cbufferEx);
		cbuf.StructureByteStride = sizeof(float);

		D3D11_SUBRESOURCE_DATA cbufData;
		cbufData.pSysMem = &t;

		GFX_THROW_INFO(pDevice->CreateBuffer(&cbuf, &cbufData, pConstantBuffer.GetAddressOf()));
	}
	else
	{
		// Retrieve data, set it, then update it
		D3D11_MAPPED_SUBRESOURCE cbufMap;
		GFX_THROW_INFO(pContext->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &cbufMap));
		memcpy(cbufMap.pData, &t, sizeof(t));
		pContext->Unmap(pConstantBuffer.Get(), 0u);
	}
	// Set the constant buffer, will associate it with the most recently set pixel shader
	pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	//////////////////////////////////////////////////////////////////////////
	// INPUT LAYOUT GOING TO THE SHADER SETUP
	//////////////////////////////////////////////////////////////////////////
	
	// Specify our input layout (after creating the shader because it'll then have to check it with the inputs to the shader)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{
			"Position", // Shader semantics hina tara
			0u,
			DXGI_FORMAT_R32G32_FLOAT,               // 3 Floats specifying a single element
			0u,
			D3D11_APPEND_ALIGNED_ELEMENT ,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u
		},
		{
			"Color",
			0u,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0u,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u
		}
	};

	GFX_THROW_INFO(pDevice->CreateInputLayout(ied.data(), UINT(ied.size()), pVSBytecodeBlob->GetBufferPointer(), pVSBytecodeBlob->GetBufferSize(), &pInputLayout));
	pContext->IASetInputLayout(pInputLayout.Get());

	//////////////////////////////////////////////////////////////////////////
	// Rasterizer State & Object Merger
	//////////////////////////////////////////////////////////////////////////

	// Here we'd set the viewport but that's only necessary if it has changd


	// Bind render targets for this draw
	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), nullptr);

	//////////////////////////////////////////////////////////////////////////
	// Set primitive topoogy then call draw
	//////////////////////////////////////////////////////////////////////////

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pContext->IASetPrimitiveTopology(topology);


	pContext->DrawIndexed(3u, 0u, 0u);

}
