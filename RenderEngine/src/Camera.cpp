#include "Camera.h"
#include "IO/Mouse.h"
#include "IO/Keyboard.h"

Camera Camera::s_Camera;

using namespace DirectX;

void Camera::Update(float dT)
{
	constexpr float moveSpeed = 15.f;
	constexpr float rotSpeed = 360.f;
	constexpr float PI = 3.142f;

	// Get move deltas for rotation and position
	std::pair<int, int> mouseMoveDelta = { m_Mouse->GetPos().first - mousePosDelta.first, m_Mouse->GetPos().second - mousePosDelta.second };
	mousePosDelta = m_Mouse->GetPos();

	std::pair<float, float> mouseDelta = { mouseMoveDelta.first * rotSpeed * dT/800.f, mouseMoveDelta.second * rotSpeed * dT/600.f };
	if (!m_Mouse->RightIsPressed() || (!bMouseInWindow && m_Mouse->IsInWindow()))
	{
		mouseDelta = { 0.f, 0.f };
	}
	bMouseInWindow = m_Mouse->IsInWindow();

	std::pair<float, float> moveDelta;
	moveDelta.first = moveSpeed * dT * (m_Keyboard->KeyIsPressed(0x57) ? 1.f : (m_Keyboard->KeyIsPressed(0x53) ? -1.f : 0.f)); // ws
	moveDelta.second = moveSpeed * dT * (m_Keyboard->KeyIsPressed(0x44) ? 1.f : (m_Keyboard->KeyIsPressed(0x41) ? -1.f : 0.f)); // ad
	const float verticalMoveDelta = moveSpeed * dT * (m_Keyboard->KeyIsPressed(0x51) ? 1.f : (m_Keyboard->KeyIsPressed(0x45) ? -1.f : 0.f)); // qe
	//~
	
	// Set rotation transforms
	m_Yaw += mouseDelta.first;
	m_Pitch = std::clamp(m_Pitch + mouseDelta.second, -PI/2.f * 0.995f, PI/2.f * 0.995f);
	const XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.f);
	const XMVECTOR forwardBaseVector = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	const auto lookVector = XMVector3Transform(forwardBaseVector, Rotation);
	// ~

	// Set position transforms
	XMVECTOR MoveDelta = XMVECTOR({ moveDelta.second, verticalMoveDelta, moveDelta.first, 1.f });
	MoveDelta = XMVector3Transform(MoveDelta, Rotation);
	m_Pos = XMVectorAdd(m_Pos, MoveDelta);
	// ~

	// Compute view matrix
	m_CameraTransform = XMMatrixLookAtLH(m_Pos, XMVectorAdd(m_Pos, lookVector), XMVectorSet(0.f, 1.f, 0.f, 0.f));

	//
}

void Camera::SetIO(Keyboard* keyboard, Mouse* mouse) noexcept
{
	m_Keyboard = keyboard;
	m_Mouse = mouse;

	mousePosDelta = m_Mouse->GetPos();
}