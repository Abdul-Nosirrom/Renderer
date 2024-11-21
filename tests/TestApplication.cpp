#include <RenderEngine.h>

#include "Graphics/RenderGraphBuilder.h"
#include "Graphics/RenderResourcePool.h"
#include "Graphics/Bindables/Sampler.h"
#include "Graphics/Bindables/Shaders.h"
#include "Graphics/Resources/Views/ResourceView.h"
#include "Scene/MeshEntity.h"
#include "Scene/Scene.h"

class TestApplication : public RenderEngine::Application
{
public:

	std::shared_ptr<MeshEntity> planeMesh;
	std::shared_ptr<MeshEntity> halfEdgeMarker;

	TestApplication()
	{
		halfEdgeMarker = std::make_shared<MeshEntity>(GeometryFactory::EType::Cube);
		halfEdgeMarker->GetTransform().SetScale(0.1f * Vector3::Ones());

		planeMesh = std::make_shared<MeshEntity>(GeometryFactory::EType::Cylinder);
		Vector3 pos(0.f, 0.f, 0.f);

		//m_Window.GFX().SetClearColor(Vector4(0.15f, 0.f, 0.f, 1.f));
		for (int i = 0; i < 4; i++)
			Scene::Get().AddCamera(std::make_shared<Camera>());
		Scene::Get().AddEntity(planeMesh);
		Scene::Get().AddEntity(halfEdgeMarker);
		//Scene::Get().LoadScene("resources/sponza.obj");
	}

	virtual int Run() override
	{
		// const HalfEdge* currentHalfEdge = planeMesh->GetMesh()->GetVertexFactory().m_HalfEdges[16];
		// unsigned int gfCount = 0;
		// int twinSwapFrameDelay = 0;

		while (true)
		{
			// Process all messages pending
			if (const auto ecode = Window::ProcessMessages())
			{
				// if return optional has a value, that means its an exit code
				return *ecode;
			}

			// Traverse half edge
			// if (gfCount%30 == 0 && currentHalfEdge)
			// {
			// 	//int vID = currentHalfEdge->vID;
			// 	Vector3 retrievedPos = currentHalfEdge->vertex->GetPosition();//planeMesh->GetMesh()->GetVertices()[vID].GetPosition();
			//
			// 	halfEdgeMarker->GetTransform().SetPosition(retrievedPos);
			// 	if (currentHalfEdge->twin && twinSwapFrameDelay == 0)
			// 	{
			// 		currentHalfEdge = currentHalfEdge->twin->next;
			// 		twinSwapFrameDelay = 2;
			// 	}
			// 	else
			// 		currentHalfEdge = currentHalfEdge->next;
			// 	//else if (currentHalfEdge)
			// 	twinSwapFrameDelay--;
			// 	if (twinSwapFrameDelay < 0) twinSwapFrameDelay = 0;
			// }

			RunFrame();

			//gfCount++;
		}
	}

	~TestApplication() override
	{

	}

	// void SetupRenderGraph() override
	// {
	// 	using namespace RenderResource;
	// 	// Early Z-Pass
	// 	RenderGraph::AddPass(L"DepthPre", []
	// 	{
	// 		auto depthTex = ResourcePool::Resolve("depth", {DXGI_FORMAT_R32_FLOAT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE});
	// 		auto dsv = BindablePool::Resolve<DepthStencilView>(depthTex);
	//
	// 		// Bind depth
	// 		Renderer::GetContext()->OMSetRenderTargets(1u, nullptr, dsv->GetDSV());
	//
	// 		// Draw opaque
	// 		Scene::Get().Render();
	// 	});
	//
	// 	// Opaque geo, depth set to read only not read/write
	// 	RenderGraph::AddPass(L"Geo", []
	// 	{
	// 		// Bind depth as erad only
	// 		auto depthTex = ResourcePool::Resolve("depth", {DXGI_FORMAT_R32_FLOAT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE});
	// 		auto dsv = DepthStencilView::Resolve(depthTex);
	//
	// 		// Create a render target
	// 		auto sceneTex = ResourcePool::Resolve("scene_color", {DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET});
	// 		auto rtv = RenderTargetView::Resolve(sceneTex);
	//
	// 		// maybe tie this to the depth stencil view because why not?
	// 		// D3D11_DEPTH_STENCIL_DESC dssDesc = {};
	// 		// dssDesc.DepthEnable = TRUE;
	// 		// dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // DONT WRITE TO DEPTH, we did in the prev pass, its read only now
	// 		// dssDesc.DepthFunc = D3D11_COMPARISON_LESS;
	// 		//
	// 		// wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	// 		//
	// 		// GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dssDesc, &pDSState));
	// 		// Renderer::GetContext()->OMSetDepthStencilState()
	//
	// 		// Bind RT & Depth
	// 		auto rtvs[] = {rtv->GetRTV()};
	// 		Renderer::GetContext()->OMSetRenderTargets(1u, rtvs, dsv->GetDSV());
	//
	// 		// Draw geo to render target
	// 		Scene::Get().Render();
	// 	});
	//
	//
	// 	// All are drawn to seperate render target, now we resolve it to the back buffer
	// 	RenderGraph::AddPass(L"Resolve", []
	// 	{
	// 		// Set render target to back-buffer (to screen)
	// 		auto backBufTex = ResourcePool::Resolve("backBuf", {DXGI_FORMAT_R32G32B32A32_FLOAT});
	// 		auto rtv = RenderTargetView::Resolve(backBufTex);
	// 		auto rtvs[] = {rtv->GetRTV()};
	//
	// 		Renderer::GetContext()->OMSetRenderTargets(1u, rtvs, nullptr);
	//
	// 		// Bind Shader
	// 		auto vertexShader = VertexShader::Resolve("shaders/fullscreenquadVS.hlsl");
	// 		auto pixelShader = PixelShader::Resolve("shaders/colorResolve.hlsl");
	//
	// 		vertexShader->Bind();
	// 		pixelShader->Bind();
	//
	// 		// Get the scene_color texture drawn before
	// 		auto sceneCol = ResourcePool::Resolve("scene_color", {DXGI_FORMAT_R32G32B32A32_FLOAT});
	// 		auto srv = ShaderResourceView::Resolve(sceneCol, ESRVBindStage::Pixel, 0u);
	// 		auto sceneSampler = Sampler::Resolve(Sampler::Type::Anisotropic, ESRVBindStage::Pixel, 0u);
	//
	// 		srv->Bind();
	// 		sceneSampler->Bind();
	//
	// 		FullScreenQuad::Draw();
	// 	});
	// }
};

RenderEngine::Application* RenderEngine::CreateApplication()
{
	return new TestApplication();
}