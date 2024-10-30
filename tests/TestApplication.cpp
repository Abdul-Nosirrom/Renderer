#include <RenderEngine.h>

#include "Scene/MeshEntity.h"
#include "Scene/Scene.h"

class TestApplication : public RenderEngine::Application
{
public:

	std::shared_ptr<MeshEntity> planeMesh;
	std::shared_ptr<MeshEntity> halfEdgeMarker;

	TestApplication()
	{
		halfEdgeMarker = std::make_shared<MeshEntity>(m_Window.GFX(), GeometryFactory::EType::Cube);
		halfEdgeMarker->GetTransform().SetScale(0.1f * Vector3::Ones());

		planeMesh = std::make_shared<MeshEntity>(m_Window.GFX(), GeometryFactory::EType::Plane);
		Vector3 pos(0.f, 0.f, 0.f);

		m_Window.GFX().SetClearColor(Vector4(0.15f, 0.f, 0.f, 1.f));
		for (int i = 0; i < 4; i++)
			Scene::Get().AddCamera(std::make_shared<Camera>());
		Scene::Get().AddEntity(planeMesh);
		Scene::Get().AddEntity(halfEdgeMarker);
		//Scene::Get().LoadScene(m_Window.GFX(), "resources/sponza.obj");
	}

	virtual int Run() override
	{
		const HalfEdge* currentHalfEdge = planeMesh->GetMesh()->GetVertexFactory().m_HalfEdges[0];
		unsigned int gfCount = 0;
		while (true)
		{
			// Process all messages pending
			if (const auto ecode = Window::ProcessMessages())
			{
				// if return optional has a value, that means its an exit code
				return *ecode;
			}

			// Traverse half edge
			if (gfCount%30 == 0 && currentHalfEdge)
			{
				//int vID = currentHalfEdge->vID;
				Vector3 retrievedPos = currentHalfEdge->vertex->GetPosition();//planeMesh->GetMesh()->GetVertices()[vID].GetPosition();

				halfEdgeMarker->GetTransform().SetPosition(retrievedPos);
				if (currentHalfEdge->twin)
					currentHalfEdge = currentHalfEdge->twin->next;
				else
					currentHalfEdge = currentHalfEdge->next;
				//else if (currentHalfEdge)
			}

			RunFrame();

			gfCount++;
		}
	}

	~TestApplication() override
	{

	}
};

RenderEngine::Application* RenderEngine::CreateApplication()
{
	return new TestApplication();
}