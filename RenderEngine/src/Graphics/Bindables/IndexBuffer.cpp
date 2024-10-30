#include "IndexBuffer.h"
#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"
#include "Primitives/VertexFactory.h"

using namespace RenderResource;

IndexBuffer::IndexBuffer(Graphics& gfx, const std::string& poolTag,
    const VertexFactory& indices)
        : m_PoolTag(poolTag), m_Count(indices.IndexCount())
{
    INFOMAN(gfx);

    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = m_Count * sizeof(UINT);
    ibd.StructureByteStride = sizeof(UINT);// sizeof(void*); Face::ByteStride();// to account for HalfEdge*

    D3D11_SUBRESOURCE_DATA ibsd = {};
    ibsd.pSysMem = indices.IndexData();

    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &ibsd, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(Graphics& gfx,
    const std::string& poolTag, const VertexFactory& indices)
{
    return Pool::Resolve<IndexBuffer>(gfx, poolTag, indices);
}

std::string IndexBuffer::GenerateUID_Internal(const std::string& poolTag)
{
    using namespace std::string_literals;
    return typeid(IndexBuffer).name() + "#"s + poolTag;
}
