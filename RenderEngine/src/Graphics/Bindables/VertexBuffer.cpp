#include "VertexBuffer.h"

#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"
#include "Primitives/VertexFactory.h"

using namespace RenderResource;

VertexBuffer::VertexBuffer(const std::string& poolTag, const VertexFactory& vertexData)
    : m_PoolTag(poolTag), m_Stride(sizeof(Vertex))
{
    INFOMAN();

    D3D11_BUFFER_DESC vbd;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.CPUAccessFlags = 0u;
    vbd.MiscFlags = 0u;
    vbd.ByteWidth = (UINT)vertexData.GetByteSize();
    vbd.StructureByteStride = m_Stride;

    D3D11_SUBRESOURCE_DATA vbsd = {};
    vbsd.pSysMem = vertexData.VertexData();

    GFX_THROW_INFO(GetDevice()->CreateBuffer(&vbd, &vbsd, &pVertexBuffer));
}

void VertexBuffer::Bind() noexcept
{
    const UINT offset = 0u;
    GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &m_Stride, &offset);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(const std::string& poolTag,
    const VertexFactory& vertexData)
{
    return BindablePool::Resolve<VertexBuffer>(poolTag, vertexData);
}

std::string VertexBuffer::GenerateUID_Internal(const std::string& poolTag)
{
    using namespace std::string_literals;
    return typeid(VertexBuffer).name() + "#"s + poolTag;
}
