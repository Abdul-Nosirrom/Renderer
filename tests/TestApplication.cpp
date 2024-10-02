#include <RenderEngine.h>

#include "Scene/Scene.h"

class TestApplication : public RenderEngine::Application
{
public:

	TestApplication()
	{
		Vector3 pos(0.f, 0.f, 0.f);

		m_Window.GFX().SetClearColor(Vector4(0.15f, 0.f, 0.f, 1.f));
		Scene::Get()->LoadScene(m_Window.GFX(), "resources/sponza.obj");
	}

	virtual int Run() override
	{
		while (true)
		{
			// Process all messages pending
			if (const auto ecode = Window::ProcessMessages())
			{
				// if return optional has a value, that means its an exit code
				return *ecode;
			}

			//ImGuiManager::NewFrame();
			RunFrame();
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