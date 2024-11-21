#pragma once
#include "Core.h"
#include "Window/Window.h"
#include "Utility/OdaTimer.h"

namespace RenderEngine
{
    class RENDERENGINE_API Application
    {
    public:
        Application();
        virtual int Run();
        virtual ~Application() = default;

    protected:
        void RunFrame();
        void Init();
        virtual void SetupRenderGraph();

        Window m_Window;
        OdaTimer m_Timer;
    };

    // Definition in Client project
    Application* CreateApplication();
}