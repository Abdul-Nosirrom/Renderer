//
// Created by Abdulrahmen on 10/3/2024.
//

#include "Editor.h"

#include <string>
#include <iostream>
#include "EditorWindow.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "Windows/Editor_Scene.h"

#define FONT_WIDTH ImGui::GetFontSize() * 0.5f

std::vector<std::string> split(std::string s, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

void Editor::CloseAllWindows()
{
    for (auto& win : Get().m_editorWindows)
    {
        win->SetIsVisible(false);
    }
}

void Editor::Initialize(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pDevice, pContext);

    // Add default windows
    m_MainMenu = new Menu();
    AddWindow<Editor_Scene>("Scene.Outliner");
    //m_editorWindows[0]->SetIsVisible(true);
}

void Editor::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

template<typename T>
std::shared_ptr<T> Editor::AddWindow(const std::string& title)
{
    std::shared_ptr<T> window = std::make_shared<T>();
    AddWindow(window, title);
    return std::move(window);
}

Editor& Editor::Get()
{
    static Editor s_instance;
    return s_instance;
}

void Editor::AddWindow(std::shared_ptr<EditorWindow> window, const std::string& title)
{
    window->SetTitle(title);
    window->Init();
    m_editorWindows.push_back(window);

    if (Menu* menu = AddMenu(title))
    {
        menu->m_Window = window.get();
    }
}

Editor::Menu* Editor::AddMenu(const std::string& name)
{
    const std::vector<std::string> Path = split(name, ".");
    Menu* CurrentMenu = m_MainMenu;

    for (auto& MenuName : Path)
    {
        int subMenuIndex = 0;
        bool bFound = false;
        for (auto subMenu : m_MainMenu->m_SubMenus)
        {
            if (subMenu->m_Name == MenuName) {bFound = true; break; }
            subMenuIndex++;
        }

        if (bFound)
        {
            CurrentMenu = CurrentMenu->m_SubMenus[subMenuIndex];
        }
        else
        {
            CurrentMenu = CurrentMenu->m_SubMenus.emplace_back(new Menu());
            CurrentMenu->m_Name = MenuName;
            //m_MainMenu->m_SubMenus.push_back(CurrentMenu);
        }
    }

    return CurrentMenu;
}

void Editor::OnRender(float dT)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
    ImGui::PopStyleColor(1);

    RenderMainMenu();

    for (auto& window : m_editorWindows)
    {
        if (window->IsVisible())
            window->RenderPrimary();
    }

    //ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(.5f, .5f), 0.2f, ImColor(255, 0, 0, 255));


    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Editor::RenderMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        for (Menu* menu : m_MainMenu->m_SubMenus)
        {
            RenderOptionMenu(*menu);
        }

        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Close All"))
                CloseAllWindows();
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Editor::RenderOptionMenu(Menu &menu)
{
    if (menu.m_Window != nullptr)
    {
        RenderMenuItem(*menu.m_Window, menu.m_Name);
    }
    else
    {
        if (ImGui::BeginMenu(menu.m_Name.c_str()))
        {
            for (Menu* subMenu : menu.m_SubMenus)
            {
                RenderOptionMenu(*subMenu);
            }
            ImGui::EndMenu();
        }
    }
}

void Editor::RenderMenuItem(EditorWindow &window, const std::string& menuItemName)
{
    ImGui::SetNextWindowSizeConstraints(
    ImVec2(FONT_WIDTH * 40, ImGui::GetTextLineHeightWithSpacing() * 5),
    ImVec2(FONT_WIDTH * 50, ImGui::GetTextLineHeightWithSpacing() * 60));

    if (ImGui::BeginMenu(menuItemName.c_str()))
    {
        window.OnRender();
        ImGui::EndMenu();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        window.SetIsVisible(!window.IsVisible());
    }

    RenderMenuItemHelp(window);
}

void Editor::RenderMenuItemHelp(EditorWindow &window)
{
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - FONT_WIDTH * 3.0f);
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(29, 42, 62, 240));
        const float HelpWidth = FONT_WIDTH * 80;
        ImGui::SetNextWindowSizeConstraints(ImVec2(HelpWidth / 2.0f, 0.0f), ImVec2(HelpWidth, FLT_MAX));
        if (ImGui::BeginTooltip())
        {
            ImGui::PushTextWrapPos(HelpWidth - 1 * FONT_WIDTH);
            window.RenderHelp();
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        ImGui::PopStyleColor();
    }
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(FONT_WIDTH * 1, 0));

}
