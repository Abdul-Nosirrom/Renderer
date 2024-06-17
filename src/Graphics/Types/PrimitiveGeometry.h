#pragma once
#include "IDrawable.h"

namespace PrimitiveGeometry
{
class Cube : public IDrawable
{
public:
    Cube(Graphics& gfx);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

    void SetPos(DirectX::XMFLOAT3 InPos) noexcept { m_Pos = InPos; }
    void SetRot(DirectX::XMFLOAT3 InRot) noexcept { m_Rot = InRot; }
    
private:
    DirectX::XMFLOAT3 m_Pos = {0.f, 0.f, 0.f};
    DirectX::XMFLOAT3 m_Rot = {0.f, 0.f, 0.f};
};
    
}

