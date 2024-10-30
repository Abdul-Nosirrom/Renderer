#pragma once

#include "MathCore.h"
#include <queue>

#include "EntityNode.h"

// temp basic camera class
class RENDERENGINE_API Camera : public EntityNode
{
	friend class Editor_Scene;
public:

	void Update(float dT);

	const Matrix4x4& GetViewMatrix() const noexcept;

	void SetActive(bool bVal) noexcept { bActive = bVal;}
	bool IsActive() const noexcept { return bActive; }

	void SetFoV(float val) { FoV = val; }
	float GetFoV() const { return FoV; }

private:

	Matrix4x4 m_ViewMatrix;

	bool bActive = false;
	bool bMouseInWindow = false;
	float FoV = 90.f;
	std::pair<int, int> mousePosDelta;
};

