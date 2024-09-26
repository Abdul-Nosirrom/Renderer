#include <RenderEngine.h>

class TestApplication : public RenderEngine::Application
{
public:

	TestApplication()
	{
		Vector3 pos(0.f, 0.f, 0.f);
		for (int i = 0; i < 1; i++)
		{
			Drawables.push_back(std::make_unique<ModelDrawable>(m_Window.GFX(), "resources/max-planck.obj"));
			Drawables[i].get()->SetPos(pos);
			pos += Vector3(10.f, 0.f, 0.f);
		}
		float ClearColor[4] = { 0.15f, 0.f, 0.f, 1.f };
		m_Window.GFX().SetClearColor(ClearColor);
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