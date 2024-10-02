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

public:
	void AddChild(const std::shared_ptr<EntityNode> &child);
	virtual void OnRender(Graphics& gfx) const {}

	const Matrix4x4& GetTransformMatrix() const noexcept override;

protected:
	void SetParent(EntityNode* parent);

	// Array of drawables? Or just a single drawable?
	Transform m_Transform;
	EntityNode* m_Parent;
	std::vector<std::shared_ptr<EntityNode>> m_childNodes;

};
