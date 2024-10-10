#include "Editor_Scene.h"

#include <format>
#include <imgui.h>

#include "Scene/EntityNode.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"

void Editor_Scene::Init()
{
    selectedEntity = nullptr;
}

void Editor_Scene::RenderHelp()
{
    ImGui::Text("Scene OutLiner, allows for modifying transforms");
}

void Editor_Scene::OnRender()
{
    auto scene = Scene::Get();

    ImGuiTableFlags table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
    if (ImGui::BeginTable("Outliner", 2, table_flags))
    {
        // Set header
        ImGui::TableSetupColumn("Outliner");
        ImGui::TableSetupColumn("Inspector");
        ImGui::TableHeadersRow();

        // Graph
        ImGui::TableNextColumn();
        if (ImGui::TreeNodeEx("Entities"))
        {
            for (auto& entity : scene.m_entities)
                RenderEntityHeierarchy(std::move(entity));
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Cameras", m_Flags))
        {
            int i = 0;
            for (auto& camera : scene.m_cameras)
            {
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
                std::string activeNotif = "";
                if (camera.get() == scene.m_activeCamera.get())
                {
                    activeNotif = "[ACTIVE]: ";
                }
                if (camera == selectedEntity) node_flags |= ImGuiTreeNodeFlags_Selected;

                std::stringstream ss;
                ss << activeNotif << "Camera #" << i;
                if (ImGui::TreeNodeEx(ss.str().c_str(), node_flags))
                {
                    if (ImGui::IsItemClicked()) selectedEntity = camera;
                    ImGui::TreePop();
                }
                i++;
            }
            ImGui::TreePop();
        }
        ImGui::TableNextColumn();
        RenderEntity();
        // Inpsection

        ImGui::EndTable();
    }
    //RenderEntity();
}

void Editor_Scene::RenderEntityHeierarchy(std::shared_ptr<class EntityNode> entity)
{
    if (!entity) return;

    auto flags = m_Flags;// | ImGuiTreeNodeFlags_Selected;
    if (entity->m_childNodes.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
    if (entity == selectedEntity) flags |= ImGuiTreeNodeFlags_Selected;

    const char* name = entity->GetName().c_str();
    if (ImGui::TreeNodeEx(name, flags))
    {
        //selectedEntity = entity;
        /*if (ImGui::TreeNodeEx("Transform"))
        {
            RenderEntity();
            ImGui::TreePop();
        }*/
        if (ImGui::IsItemClicked())
        {
            selectedEntity = entity;
        }

        for (auto& child : entity->m_childNodes)
            RenderEntityHeierarchy(std::move(child));
        ImGui::TreePop();
    }
}

void Editor_Scene::RenderEntity()
{
    if (!selectedEntity) return;

    std::string propertyTitle = std::format("Entity Properties [%s]", selectedEntity->GetName());
    //ImGui::BeginChild(propertyTitle.c_str());
    auto& transform = selectedEntity->m_Transform;
    RenderTransform(transform);

    if (std::static_pointer_cast<Camera>(selectedEntity))
    {
        RenderCamera();
    }

    //ImGui::EndChild();
}

void Editor_Scene::RenderCamera()
{
    auto camera = std::static_pointer_cast<Camera>(selectedEntity);

    ImGui::SliderFloat("FoV", &camera->FoV, 20.f, 140.f, "%.0f deg");

    if (ImGui::Checkbox("Active:", &camera->bActive))
    {
        Scene::Get().SetActiveCamera(camera);
    }
}

void Editor_Scene::RenderTransform(class Transform &transform)
{
    // Position
    if (ImGui::InputFloat3("Position: ", transform.m_Position.data())) transform.UpdateTransformMatrix();

    // Rotation
    if (ImGui::SliderFloat3("Rotation: ", transform.m_Rotation.data(), -180.0f, 180.0f)) transform.UpdateTransformMatrix();

    // Scale
    if (ImGui::InputFloat3("Scale: ", transform.m_Scale.data())) transform.UpdateTransformMatrix();

    // Matrix

}
