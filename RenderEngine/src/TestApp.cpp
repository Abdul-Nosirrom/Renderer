#include "TestApp.h"

#include <chrono>
#include <random>
#include "Camera.h"
//#include "ImGuiContext.h"

RenderEngine::Application::Application()
    : m_Window(800, 800, "Renderer")
{
}

int RenderEngine::Application::Run()
{
    return 0;
}

void RenderEngine::Application::RunFrame()
{
	const float dt = m_Timer.Mark();
	Camera::Get().Update(dt);


	m_Window.GFX().StartFrame();

	for (auto& Drawable : Drawables)
	{
		Drawable->Update(m_Window.GFX(), dt);
		Drawable->Draw(m_Window.GFX());
	}

	//ImGuiManager::EndFrame();
	m_Window.GFX().EndFrame();
}
