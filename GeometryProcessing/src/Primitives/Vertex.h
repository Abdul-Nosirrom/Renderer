#pragma once
#include <d3d11.h>
#include <dxgiformat.h>

#include "GeometryProcessing.h"
#include "Math/Vector.h"

#define ATTRIB_DESC(Attribute, Format)\
static constexpr D3D11_INPUT_ELEMENT_DESC AttributeDesc_##Attribute = \
    { #Attribute, 0u, Format, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u };

#define VERTEX_SETTER(Attribute, SysType)\
    void Set##Attribute##(const SysType##& In) noexcept { ##Attribute## = In; }
#define VERTEX_GETTER(Attribute, SysType)\
    const SysType& Get##Attribute##() const noexcept { return Attribute##; }
class GEO_API VertexAttributes
{
public:
    ATTRIB_DESC(TexCoord, DXGI_FORMAT_R32G32_FLOAT);
    ATTRIB_DESC(Position, DXGI_FORMAT_R32G32B32_FLOAT);
    ATTRIB_DESC(Normal, DXGI_FORMAT_R32G32B32_FLOAT);
    ATTRIB_DESC(Tangent, DXGI_FORMAT_R32G32B32_FLOAT);
    ATTRIB_DESC(Color, DXGI_FORMAT_R32G32B32_FLOAT);
    ATTRIB_DESC(Data, DXGI_FORMAT_R32G32B32A32_FLOAT);

    inline static constexpr UINT NumElements = 6;
    inline static constexpr D3D11_INPUT_ELEMENT_DESC InputLayout[NumElements] =
    { AttributeDesc_TexCoord, AttributeDesc_Position, AttributeDesc_Normal, AttributeDesc_Tangent, AttributeDesc_Color, AttributeDesc_Data };
};

struct GEO_API Vertex
{
    Vertex() noexcept = default;

    VERTEX_SETTER(UV, Vector2);
    VERTEX_SETTER(Position, Vector3);
    VERTEX_SETTER(Normal, Vector3);
    VERTEX_SETTER(Tangent, Vector3);
    VERTEX_SETTER(Color, Vector3);
    VERTEX_SETTER(Data, Vector4);

    VERTEX_GETTER(UV, Vector2);
    VERTEX_GETTER(Position, Vector3);
    VERTEX_GETTER(Normal, Vector3);
    VERTEX_GETTER(Tangent, Vector3);
    VERTEX_GETTER(Color, Vector3);
    VERTEX_GETTER(Data, Vector4);

    bool operator==(const Vertex & other) const
    {
        return UV == other.UV && Position == other.Position && Normal == other.Normal && Tangent == other.Tangent
                && Color == other.Color && Data == other.Data;
    }

    Vector2 UV;
    Vector3 Position;
    Vector3 Normal;
    Vector3 Tangent;
    Vector3 Color;
    Vector4 Data;
};

class GEO_API VertexArray
{
public:
    std::vector<Vertex> m_Vertices;
};

inline void GEO_API VertexMatrix(const VertexArray& vBuf, Eigen::MatrixXd& V)
{
    auto& vertices = vBuf.m_Vertices;

    V.resize(vertices.size(), 3);

    for (int i = 0; i < vertices.size(); i++)
    {
        V(i, 0) = vertices[i].Position[0];
        V(i, 1) = vertices[i].Position[1];
        V(i, 2) = vertices[i].Position[2];
    }
}