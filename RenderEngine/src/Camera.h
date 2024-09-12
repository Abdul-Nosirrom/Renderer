#pragma once

#include <DirectXMath.h>
#include <queue>

// temp basic camera class
class Camera
{
public:
	static Camera& Get() { return s_Camera; }

private:
	Camera() noexcept
	{
		m_Pos = DirectX::XMVECTOR({ 0, 0, -5.f, 1 });
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

	DirectX::XMMATRIX GetTransform() { return m_CameraTransform;  }

private:
	DirectX::XMMATRIX m_CameraTransform;

	DirectX::XMVECTOR m_Pos;
	float m_Pitch, m_Yaw;
	bool bMouseInWindow;
	std::pair<int, int> mousePosDelta;

	Keyboard* m_Keyboard;
	Mouse* m_Mouse;
};

