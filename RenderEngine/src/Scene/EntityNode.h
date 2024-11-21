//
// Created by Abdulrahmen on 9/27/2024.
//
#pragma once
#include "Core.h"
#include <vector>
#include "Graphics/Types/IDrawable.h"
#include "Math/Transform.h"


class RENDERENGINE_API EntityNode : public IDrawable
{
	friend class Scene;
	friend class Editor_Scene;

public:
	void AddChild(const std::shared_ptr<EntityNode> &child);
	virtual void OnRender() const {}

	const Matrix4x4& GetTransformMatrix() const noexcept override;
	Transform& GetTransform() noexcept { return m_Transform; }
	virtual const std::string& GetName() const { return ""; }

protected:
	void SetParent(EntityNode* parent);

	// Array of drawables? Or just a single drawable?
	Transform m_Transform = Transform::Identity;
	EntityNode* m_Parent = nullptr;
	std::vector<std::shared_ptr<EntityNode>> m_childNodes;

};
