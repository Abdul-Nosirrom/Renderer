#include "Application.h"
#include "MathCore.h"
#include <chrono>
#include <random>
#include "Scene/Camera.h"
#include <iostream>

#include "Editor/Editor.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderGraphBuilder.h"
#include "Graphics/RenderResourcePool.h"
#include "Graphics/Resources/Views/ResourceView.h"
#include "Scene/Scene.h"
#include "Graphics/Bindables/BindablesCommon.h"
#include "Graphics/Meshes/Mesh.h"
//#include "ImGuiContext.h"

RenderEngine::Application::Application()
    : m_Window(800, 800, "Renderer")
{
	Matrix4x4 A;
	A << 1, 0, 1, 0,
		0, 0, 1, 0,
		1, 2, 0, 0,
		0, 1, 1, 1;

	Matrix4x4 B;
	B << 0, 2, 2, 1,
		1, 1, 0, 0,
		0, 1, 2, 3,
		0, 0, 0, 1;

	std::cout << "Trasnlation Matrix = " << std::endl << MatrixConstruction::TranslationMatrix({ 2.f, 5.f, 8.f }) << std::endl;

	std::cout << std::endl << "A = " << std::endl << A << std::endl;
	std::cout << std::endl << "B = " << std::endl << B << std::endl;

	std::cout << std::endl << "A * B = " << std::endl << A * B << std::endl;

	Vector3 Trans(2.f, 3.f, 4.f);
	Vector3 Forward(1.f, 0.f, 0.f);
	Vector3 Up(0.f, 1.f, 0.f);

	Init();
}

int RenderEngine::Application::Run()
{
    return 0;
}

void RenderEngine::Application::RunFrame()
{
	const float dt = m_Timer.Mark();
	Scene::Get().Update(dt);

	Renderer::StartFrame();

	RenderGraph::Execute();

	// ImGui fixed render loop
	{
		RENDER_EVENT(EditorDraw, L"EditorImGui")
		Editor::Get().OnRender(dt);
	}

	Renderer::EndFrame();
}

void RenderEngine::Application::Init()
{
	SetupRenderGraph();
}

void BindGlobalConstantBuffers()
{

}

void RenderEngine::Application::SetupRenderGraph()
{
	using namespace RenderResource;

	auto BindGlobals = [this]()
	{
		struct Time
		{
			alignas(16) float time;
		};

		struct CameraData
		{
			alignas(16) Vector3 Pos;
			alignas(16) Vector3 Dir;
		};

		auto activeCam = Scene::Get().GetActiveCamera()->GetTransform();
		auto camDir = MatrixConstruction::RotationMatrix(activeCam.GetRotation()) * Vector3(0,0,1);
		auto camPos = activeCam.GetPosition();

		Time t = { m_Timer.Time() };
		CameraData cam = { camPos, camDir };

		auto timeCBuf = PixelConstantBuffer<Time>::Resolve(0);
		timeCBuf->Update(t);
		//auto camCBuf = PixelConstantBuffer<CameraData>::Resolve(1);
		//camCBuf->Update(cam);

		//camCBuf->Bind();
		timeCBuf->Bind();
	};

	RenderGraph::AddPass(L"Clear", []()
	{
		Renderer::SetRenderViewportSize(Renderer::EViewportSize::FULL);

		BindablePool::ClearRenderTargetViews({0.f, 0.f, 0.f, 0.f});
		BindablePool::ClearDepthStencilViews(1, 0);
	});

	RenderGraph::AddPass(L"Scene", []()
	{
		// // Set render target
		// auto pBackBuffer = ResourcePool::Resolve("back_buffer", {DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET});
		Renderer::SetRenderViewportSize(Renderer::EViewportSize::FULL);

		auto colTex = ResourcePool::Resolve("scene_color", {DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE});
		auto colRTV = RenderTargetView::Resolve(colTex.get())->GetRTV();

		auto posTex = ResourcePool::Resolve("world_pos", {DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE});
		auto posRTV = RenderTargetView::Resolve(posTex.get())->GetRTV();

		auto normTex = ResourcePool::Resolve("world_norm", {DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE});
		auto normRTV = RenderTargetView::Resolve(normTex.get())->GetRTV();

		// Bind depth stencil
		auto pDepthTex = ResourcePool::Resolve("depth", {DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL});
		auto pDSV = DepthStencilView::Resolve(pDepthTex.get());
		auto pDepthState = DepthStencilState::Resolve({TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS});

		pDepthState->Bind();

		ID3D11RenderTargetView* rtvs[3] = {colRTV, posRTV, normRTV};
		Renderer::GetContext()->OMSetRenderTargets(3u, rtvs, pDSV->GetDSV());
		//
		// pBBRTV->Bind();
		// Draw opaque geo
		Scene::Get().Render();
	});

	RenderGraph::AddPass(L"Resolve", [BindGlobals]()
	{
		BindGlobals();

		Renderer::SetRenderViewportSize(Renderer::EViewportSize::FULL);

		// Unbind depth
		auto pBBRTV = Renderer::GetBackBuffer();
		Renderer::GetContext()->OMSetRenderTargets(1u, &pBBRTV, nullptr);

		Mesh* mesh = new Mesh(GeometryFactory::EType::Plane);
		auto vbuf = VertexBuffer::Resolve(mesh->GetPoolTag(), mesh->GetVertexFactory());
		auto ibuf = IndexBuffer::Resolve(mesh->GetPoolTag(), mesh->GetVertexFactory());

		vbuf->Bind();
		ibuf->Bind();

		// Bind pixel shader texture & sampler
		auto colTex = ResourcePool::Resolve("scene_color", {DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE});
		auto colSRV = ShaderResourceView::Resolve(colTex.get(), ESRVBindStage::Pixel, 0);

		auto posTex = ResourcePool::Resolve("world_pos", {DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE});
		auto posSRV = ShaderResourceView::Resolve(posTex.get(), ESRVBindStage::Pixel, 1);

		auto normTex = ResourcePool::Resolve("world_norm", {DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE});
		auto normSRV = ShaderResourceView::Resolve(normTex.get(), ESRVBindStage::Pixel, 2);


		auto colSampler = Sampler::Resolve(Sampler::Type::Anisotropic, ESRVBindStage::Pixel, 0);
		auto posSampler = Sampler::Resolve(Sampler::Type::Anisotropic, ESRVBindStage::Pixel, 1);
		auto normSampler = Sampler::Resolve(Sampler::Type::Anisotropic, ESRVBindStage::Pixel, 2);

		colSRV->Bind();
		posSRV->Bind();
		normSRV->Bind();

		colSampler->Bind();
		posSampler->Bind();
		normSampler->Bind();

		auto pVS = VertexShader::Resolve("shaders/fullscreenquad.hlsl");
		auto pPS = PixelShader::Resolve("shaders/fullscreenquad.hlsl");

		pVS->Bind();
		pPS->Bind();

		auto topology = Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		topology->Bind();

		auto inputlayout = InputLayout::Resolve();
		inputlayout->Bind();

		Renderer::Draw(ibuf->GetCount());
	});
}
