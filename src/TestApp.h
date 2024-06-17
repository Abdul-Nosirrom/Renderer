#pragma once
#include "Window.h"
#include "Graphics/Types/PrimitiveGeometry.h"

class TestApp
{
public:
    TestApp();
    int Go();
    ~TestApp();

private:
    void Render();

private:
    Window m_Window;
    std::vector<std::unique_ptr<PrimitiveGeometry::Cube>> Drawables;
};
