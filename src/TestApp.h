#pragma once
#include "Window.h"
#include "Graphics/Types/PrimitiveGeometry.h"
#include "OdaTimer.h"

class TestApp
{
public:
    TestApp();
    int Go();
    ~TestApp();

private:
    void RunFrame();

private:
    Window m_Window;
    OdaTimer m_Timer;
    std::vector<std::unique_ptr<PrimitiveDrawable>> Drawables;
};
