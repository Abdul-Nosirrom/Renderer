//
// Created by Abdulrahmen on 10/17/2024.
//

#include "Matrix.h"

Matrix4x4 MatrixConstruction::ViewTransform(const Vector3& _Pos, const Vector3& _FocusVec, const Vector3& _Up)
{
    // why left handed?
    const Vector3 z(_FocusVec.normalized());
    const Vector3 x((-z.cross(_Up)).normalized());
    const Vector3 y((-x.cross(z)).normalized());

    const float DX = x.dot(-_Pos);
    const float DY = y.dot(-_Pos);
    const float DZ = z.dot(-_Pos);


    Matrix4x4 viewMatrix(Matrix4x4::Zero(4, 4));

    // Rotation matrix with camera basis
    viewMatrix.row(0) = Homogenize(x, 0.f);
    viewMatrix.row(1) = Homogenize(y, 0.f);
    viewMatrix.row(2) = Homogenize(z, 0.f);
    viewMatrix.col(3) = Homogenize({DX, DY, DZ}, 1.f); // inverse pos

    return viewMatrix;
}

Matrix4x4 MatrixConstruction::PerspectiveMatrix(float fovX, float aspectRatio, float zNear, float zFar)
{
    const float tanFOV = tan(DEG_TO_RAD(fovX / 2.f));

    Matrix4x4 projMatrix;
    projMatrix <<	1 / tanFOV,	0.f,						0.f,				0.f,
                    0.f,		1 / (aspectRatio * tanFOV), 0.f,				0.f,
                    0.f,		0.f,						zFar/(zFar-zNear),	-zFar * zNear / (zFar - zNear),
                    0.f,		0.f,						1.f,				0.f;

    return projMatrix;
}