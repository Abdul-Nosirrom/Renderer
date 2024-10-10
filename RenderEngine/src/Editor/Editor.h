#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include <wrl/client.h>
#include "Core.h"
#include "Scene/Scene.h"

class EditorWindow;
namespace RenderEngine
{
    class Application;
}

/// @brief  Singleton
class RENDERENGINE_API Editor
{
    friend RenderEngine::Application;
    friend class Window;
public:
    static Editor& Get();

    template<typename T>
    std::shared_ptr<T> AddWindow(const std::string& title);
    void AddWindow(std::shared_ptr<EditorWindow> window, const std::string& title);

    static void CloseAllWindows();

protected:

    void Initialize(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Shutdown();

    struct Menu
    {
        std::string m_Name;
        EditorWindow* m_Window;
        std::vector<Menu*> m_SubMenus;
    };
    Menu* AddMenu(const std::string& name);

    Menu* m_MainMenu = nullptr;

    void OnRender(float dT);
    void RenderMainMenu();
    void RenderOptionMenu(Menu& menu);
    void RenderMenuItem(EditorWindow& window, const std::string& menuItemName);
    void RenderMenuItemHelp(EditorWindow& window);
private:

    std::vector<std::shared_ptr<EditorWindow>> m_editorWindows;

    //inline static Editor* s_Instance = nullptr;
};