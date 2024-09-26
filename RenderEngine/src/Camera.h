#pragma once

#include "Math/Math.h"
#include <queue>

// temp basic camera class
class Camera
{
	friend class Graphics;
public:
	static Camera& Get() { return s_Camera; }

private:
	Camera() noexcept
	{
		m_Pos = Vector3( 0, 0, -5.f );
		m_Yaw = 0.f;
		m_Pitch = 0.f;
		m_Keyboard = nullptr;
		m_Mouse = nullptr;
		bMouseInWindow = false;
	}
	~Camera() = default;
	
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	// singleton
	static Camera s_Camera;

public:

	void Update(float dT);

	void SetIO(class Keyboard* keyboard, class Mouse* mouse) noexcept;

	Matrix4x4 GetTransform() { return m_CameraTransform;  }

private:
	Matrix4x4 m_CameraTransform;

	Vector3 m_Pos;
	float m_Pitch, m_Yaw;
	bool bMouseInWindow;
	std::pair<int, int> mousePosDelta;

	Keyboard* m_Keyboard;
	Mouse* m_Mouse;
};

