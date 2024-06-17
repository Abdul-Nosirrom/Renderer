#include "PrimitiveGeometry.h"

#include "Graphics/Bindables/BindablesCommon.h"

#include <iostream>

using namespace PrimitiveGeometry;
using namespace RenderResource;

Cube::Cube(Graphics& gfx)
{
    const auto geoTag = "$cube.";
    
    VertexFactory cubeDescriptor;
    {
        cubeDescriptor.AddVertex({-1, -1, -1});
        cubeDescriptor.AddVertex({1, -1, -1});
        cubeDescriptor.AddVertex({-1, 1, -1});
        cubeDescriptor.AddVertex({1, 1, -1});
        cubeDescriptor.AddVertex({-1, -1, 1});
        cubeDescriptor.AddVertex({1, -1, 1});
        cubeDescriptor.AddVertex({-1, 1, 1});
        cubeDescriptor.AddVertex({1, 1, 1});
    }

    const std::vector<unsigned short> idxData =
    {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };

    // Geo Buffers
    AddBindable(VertexBuffer::Resolve(gfx, geoTag, cubeDescriptor));
    AddBindable(IndexBuffer::Resolve(gfx, geoTag, idxData));

    // Shaders
    auto pVS = VertexShader::Resolve(gfx, "shaders/Shaders.hlsl");
    auto pVSByteCode = pVS->GetBytecode(); // Retrieve to feed input input layout
    AddBindable(std::move(pVS));
    
    AddBindable(PixelShader::Resolve(gfx, "shaders/Shaders.hlsl"));

    // Shader Data
    AddBindable(InputLayout::Resolve(gfx, pVSByteCode));

    // Topology
    AddBindable(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    
    // Constant Buffers
    AddBindable(std::make_shared<TransformCBuffer>(gfx, *this));
}

DirectX::XMMATRIX Cube::GetTransformMatrix() const noexcept
{
    return DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z) * DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
}
