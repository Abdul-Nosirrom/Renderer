#include "TestApp.h"

#include <chrono>

#include "Camera.h"

TestApp::TestApp()
    : m_Window(800, 600, "Renderer")
{
    for (int i = 0; i <= 25; i++)
    {
        Drawables.push_back(std::make_unique<PrimitiveGeometry::Cube>(m_Window.GFX()));
        Drawables[i]->SetPos({i * 5.f, 0.f, 0.f});
        Drawables[i]->SetRot({0.f, 0.f, i * 360.f / 25.f});
    }

    float ClearColor[4] = { 0.5, 0, 0, 1 };
    m_Window.GFX().SetClearColor(ClearColor);
}

int TestApp::Go()
{
    // BEGIN time tests
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    float dt = 0.f;
    // ~
    
    while (true)
    {
        auto t0 = Time::now();

        // Process all messages pending
        if (const auto ecode = Window::ProcessMessages())
        {
            // if return optional has a value, that means its an exit code
            return *ecode;
        }

        Camera::Get().Update(dt);
        Render();

        auto t1 = Time::now();
        fsec tInS = t1 - t0;
        dt = tInS.count();
    }
}

TestApp::~TestApp()
{}

void TestApp::Render()
{
    m_Window.GFX().StartFrame();

    for (auto& Drawable : Drawables)
    {
        Drawable->Draw(m_Window.GFX());
    }

    m_Window.GFX().EndFrame();
}
