//
// Created by Abdulrahmen on 9/27/2024.
//

#include "EntityNode.h"

void EntityNode::AddChild(const std::shared_ptr<EntityNode>& child)
{
	child->SetParent(this);
	m_childNodes.push_back(child);
}

const Matrix4x4& EntityNode::GetTransformMatrix() const noexcept
{
	if (m_Parent != nullptr) return m_Parent->GetTransformMatrix() * m_Transform.GetMatrix();
	return m_Transform.GetMatrix();
}

void EntityNode::SetParent(EntityNode* parent)
{
	m_Parent = parent;
}
