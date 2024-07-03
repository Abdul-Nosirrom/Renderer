#include "TestApp.h"

#include <chrono>
#include <random>
#include "Camera.h"

TestApp::TestApp()
    : m_Window(800, 600, "Renderer")
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> typeDist(0, 3); 

    for (UINT i = 0; i < 50; i++)
    {
        PrimitiveMesh::EType instType = static_cast<PrimitiveMesh::EType>(typeDist(rng));
        Drawables.push_back(std::make_unique<PrimitiveDrawable>(m_Window.GFX(), instType));
    }


    float ClearColor[4] = { 0.15f, 0.f, 0.f, 1.f };
    m_Window.GFX().SetClearColor(ClearColor);
}

int TestApp::Go()
{
    while (true)
    {
        // Process all messages pending
        if (const auto ecode = Window::ProcessMessages())
        {
            // if return optional has a value, that means its an exit code
            return *ecode;
        }

        RunFrame();
    }
}

TestApp::~TestApp()
{}

void TestApp::RunFrame()
{
    const float dt = m_Timer.Mark();
	Camera::Get().Update(dt);


    m_Window.GFX().StartFrame();

    for (auto& Drawable : Drawables)
    {
        Drawable->Update(m_Window.GFX(), dt);
        Drawable->Draw(m_Window.GFX());
    }

    m_Window.GFX().EndFrame();
}
