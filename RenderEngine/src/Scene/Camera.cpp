#include "Camera.h"
#include "Window/IO/Mouse.h"
#include "Window/IO/Keyboard.h"
#include <iostream>
#include <Windows.h>

#include "Window/IO/InputHandler.h"

void Camera::Update(float dT)
{
	constexpr float moveSpeed = 15.f;
	constexpr float rotSpeed = 15.f;

	Mouse& m_Mouse = InputHandler::GetMouse();
	Keyboard& m_Keyboard = InputHandler::GetKeyboard();
	float m_Yaw = m_Transform.GetRotation().y();
	float m_Pitch = m_Transform.GetRotation().x();

	// Get move deltas for rotation and position
	std::pair<int, int> mouseMoveDelta = { m_Mouse.GetPos().first - mousePosDelta.first, m_Mouse.GetPos().second - mousePosDelta.second };
	mousePosDelta = m_Mouse.GetPos();

	std::pair<float, float> mouseDelta = { mouseMoveDelta.first * rotSpeed * dT, mouseMoveDelta.second * rotSpeed * dT };
	if (!m_Mouse.RightIsPressed() || (!bMouseInWindow && m_Mouse.IsInWindow()))
	{
		mouseDelta = { 0.f, 0.f };
	}
	bMouseInWindow = m_Mouse.IsInWindow();

	std::pair<float, float> moveDelta;
	moveDelta.first = moveSpeed * dT * (m_Keyboard.KeyIsPressed(VK_UP) ? 1.f : (m_Keyboard.KeyIsPressed(VK_DOWN) ? -1.f : 0.f)); // ws
	moveDelta.second = moveSpeed * dT * (m_Keyboard.KeyIsPressed(VK_RIGHT) ? 1.f : (m_Keyboard.KeyIsPressed(VK_LEFT) ? -1.f : 0.f)); // da
	const float verticalMoveDelta = moveSpeed * dT * (m_Keyboard.KeyIsPressed(VK_PRIOR) ? 1.f : (m_Keyboard.KeyIsPressed(VK_NEXT) ? -1.f : 0.f)); // qe
	//~
	
	// Set rotation transforms
	m_Yaw += mouseDelta.first;
	m_Pitch = std::clamp(m_Pitch + mouseDelta.second, -90 * 0.995f, 90 * 0.995f);
	m_Transform.SetRotation({m_Pitch, m_Yaw, 0.f});
	const Matrix3x3 rotation = MatrixConstruction::RotationMatrix(m_Transform.GetRotation());
	const Vector3 forwardBaseVector(0.f, 0.f, 1.f);
	const Vector3 lookVector = (rotation * forwardBaseVector);
	// ~

	// Set position transforms
	Vector3 MoveDelta(moveDelta.second, verticalMoveDelta, moveDelta.first);
	MoveDelta = rotation * MoveDelta; //NOTE: for some reason only adding the local space delta works? Making it into world space then adding causes it to *not* work as expected This is an issue in the ViewTransform matrix construction...
	m_Transform.SetPosition(m_Transform.GetPosition() + (MoveDelta));
	// ~

	// Compute view matrix
	m_ViewMatrix = MatrixConstruction::ViewTransform(m_Transform.GetPosition(), lookVector, Vector3(0.f, 1.f, 0.f));
	//
}

const Matrix4x4& Camera::GetViewMatrix() const noexcept
{
	return m_ViewMatrix;
}
