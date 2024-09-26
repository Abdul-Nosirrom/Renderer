#pragma once

#include "Core.h"
#include <Eigen/Core>
#include <Eigen/Eigen>

using Matrix4x4 = Eigen::Matrix<float, 4, 4>;
using Matrix3x3 = Eigen::Matrix<float, 3, 3>;
using Vector4	= Eigen::Vector<float, 4>;
using Vector3	= Eigen::Vector<float, 3>;
using Vector2	= Eigen::Vector<float, 2>;

#define PI 3.14159265358979323846f
#define DEG_TO_RAD(x) x * PI / 180.f
#define RAD_TO_DEG(x) x * 180.f / PI


namespace MatrixConstruction
{
	RENDERENGINE_API Matrix4x4 TranslationMatrix(const Vector3& _Translation);
	RENDERENGINE_API Matrix3x3 ScaleMatrix(const Vector3& _Scale);
	RENDERENGINE_API Matrix3x3 RotationMatrix(const Vector3& _Angles);
	RENDERENGINE_API Matrix3x3 Rotation_X(float _Angle);
	RENDERENGINE_API Matrix3x3 Rotation_Y(float _Angle);
	RENDERENGINE_API Matrix3x3 Rotation_Z(float _Angle);

	RENDERENGINE_API Matrix4x4 ViewTransform(const Vector3& _Pos, const Vector3& _FocusVec, const Vector3& _Up);

	RENDERENGINE_API Matrix4x4 PerspectiveMatrix(float fovX, float aspectRatio, float zNear, float zFar);

	inline RENDERENGINE_API Vector4 Homogenize(const Vector3& _vec, float _w)
	{
		return Vector4(_vec(0), _vec(1), _vec(2), _w);
	}

	inline RENDERENGINE_API Matrix4x4 Homogenize(const Matrix3x3& _Mat) 
	{ Matrix4x4 _homoMat = Matrix4x4::Zero(4, 4); _homoMat.topLeftCorner(3, 3) = _Mat; _homoMat(15) = 1.f; return _homoMat; }

	inline RENDERENGINE_API Matrix4x4 TransformationMatrix(const Vector3& _Translation, const Vector3& _Rotation, const Vector3& _Scale)
	{
		return TranslationMatrix(_Translation) * Homogenize(RotationMatrix(_Rotation) * ScaleMatrix(_Scale));
	}

	inline RENDERENGINE_API Matrix4x4 TranslationMatrix(const Vector3& _Translation)
	{
		Matrix4x4 T = Matrix4x4::Identity(4, 4);
		T.col(3) = Homogenize(_Translation, 1.f);
		return T;
	}

	inline RENDERENGINE_API Matrix3x3 ScaleMatrix(const Vector3& _Scale)
	{
		Matrix3x3 S = Matrix3x3::Identity(3, 3);
		S.diagonal() = _Scale;
		return S;
	}

	inline RENDERENGINE_API Matrix3x3 RotationMatrix(const Vector3& _Angles)
	{
		// Y / P / R
		return Rotation_Z(_Angles(2)) * Rotation_Y(_Angles(1)) * Rotation_X(_Angles(0));
	}

	inline RENDERENGINE_API Matrix3x3 Rotation_X(float _Angle)
	{
		_Angle = DEG_TO_RAD(_Angle);
		Matrix3x3 Rx;
		Rx <<	1.f, 0.f,			0.f,	
				0.f, cos(_Angle), -sin(_Angle),
				0.f, sin(_Angle), cos(_Angle);
		return Rx;
	}

	inline RENDERENGINE_API Matrix3x3 Rotation_Y(float _Angle)
	{
		_Angle = DEG_TO_RAD(_Angle);
		Matrix3x3 Ry;
		Ry <<	cos(_Angle),	0.f, sin(_Angle),
				0.f,			1.f, 0.f,
				-sin(_Angle),	0.f, cos(_Angle);
		return Ry;
	}

	inline RENDERENGINE_API Matrix3x3 Rotation_Z(float _Angle)
	{
		_Angle = DEG_TO_RAD(_Angle);
		Matrix3x3 Rz;
		Rz <<	cos(_Angle),	-sin(_Angle),	0.f,
				sin(_Angle),	cos(_Angle),	0.f,
				0.f,			0.f,			1.f;
		return Rz;
	}
};