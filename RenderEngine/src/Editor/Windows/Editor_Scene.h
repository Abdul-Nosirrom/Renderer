#pragma once
#include <memory>

#include "imgui.h"
#include "Editor/EditorWindow.h"


class RENDERENGINE_API Editor_Scene : public EditorWindow
{
public:
    void Init() override;

    void RenderHelp() override;

    void OnRender() override;
    void RenderEntityHeierarchy(std::shared_ptr<class EntityNode> entity);
    void RenderEntity();
    void RenderCamera();

protected:
    void RenderTransform(class Transform& transform);

    ImGuiTreeNodeFlags m_Flags = 0;
    std::shared_ptr<EntityNode> selectedEntity = nullptr;
};
