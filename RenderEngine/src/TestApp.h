#pragma once
#include "Core.h"
#include "Window.h"
#include "Graphics/Types/PrimitiveGeometry.h"
#include "OdaTimer.h"

namespace RenderEngine
{
    class RENDERENGINE_API Application
    {
    public:
        Application();
        virtual int Run();
        virtual ~Application() {};

    protected:
        void RunFrame();

        Window m_Window;
        OdaTimer m_Timer;
        std::vector<std::unique_ptr<PrimitiveDrawable>> Drawables;
    };

    // Definition in Client project
    Application* CreateApplication();
}