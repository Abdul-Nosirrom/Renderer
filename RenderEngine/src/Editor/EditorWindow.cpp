//
// Created by Abdulrahmen on 10/3/2024.
//

#include "EditorWindow.h"

#include "imgui.h"

void EditorWindow::RenderPrimary()
{
    ImGuiWindowFlags WindowFlags = 0;

    //if (bHasMenu && bHideMenu == false)
    {
        WindowFlags |= ImGuiWindowFlags_MenuBar;
    }

    if (ImGui::Begin(m_Title.c_str(), &bVisibility, WindowFlags))
    {

        if (ImGui::BeginPopupContextWindow())
        {
            //if (bHasMenu)
            {
                ImGui::Checkbox("Hide Menu", &bVisibility);
            }

            ImGui::EndPopup();
        }

        OnRender();
    }
    ImGui::End();
}
