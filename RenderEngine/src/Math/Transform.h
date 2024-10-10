//
// Created by Abdulrahmen on 9/27/2024.
//
#pragma once
#include "Math.h"


class RENDERENGINE_API Transform
{
	friend class Editor_Scene;
public:
	Transform() : m_Position(Vector3(0,0,0)), m_Rotation(Vector3(0,0,0)), m_Scale(Vector3(1,1,1)) {}
	Transform(const Vector3& inPos, const Vector3& inRot, const Vector3& inScale) : m_Position(inPos), m_Rotation(inRot), m_Scale(inScale) {}

	inline void SetPosition(const Vector3& position) { m_Position = position; UpdateTransformMatrix(); }
	inline void SetRotation(const Vector3& rotation) { m_Rotation = rotation; UpdateTransformMatrix(); }
	inline void SetScale(const Vector3& scale) { m_Scale = scale; UpdateTransformMatrix(); }
	inline void SetMatrix(const Matrix4x4& matrix) { m_Transform = matrix; } // is there a way to derive pos/rot/scale from the matrix?

	inline const Vector3& GetPosition() const { return m_Position; }
	inline const Vector3& GetRotation() const { return m_Rotation; }
	inline const Vector3& GetScale() const { return m_Scale; }
	inline const Matrix4x4& GetMatrix() const { return m_Transform; }

	bool IsIdentity() const { return *this == Identity; }

	bool operator==(const Transform& Other) const
	{
		return m_Position == Other.GetPosition() && m_Rotation == Other.GetRotation() && m_Scale == Other.GetScale();
	}

	bool operator!=(const Transform& Other) const
	{
		return !(*this == Other);
	}

	static Transform Identity;

private:

	inline void UpdateTransformMatrix()
	{
		m_Transform = MatrixConstruction::TransformationMatrix(m_Position, m_Rotation, m_Scale);
	}

	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;

	Matrix4x4 m_Transform;
};

#if BUILD_DLL
inline Transform Transform::Identity = Transform();
#endif