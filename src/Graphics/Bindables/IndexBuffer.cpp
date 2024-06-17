#include "IndexBuffer.h"

#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"

using namespace RenderResource;

IndexBuffer::IndexBuffer(Graphics& gfx, const std::string& poolTag,
    const std::vector<unsigned short>& indices)
        : m_PoolTag(poolTag), m_Count(indices.size())
{
    INFOMAN(gfx);

    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = m_Count * sizeof(unsigned short);
    ibd.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA ibsd = {};
    ibsd.pSysMem = indices.data();

    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &ibsd, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(Graphics& gfx,
    const std::string& poolTag, const std::vector<unsigned short>& indices)
{
    return Pool::Resolve<IndexBuffer>(gfx, poolTag, indices);
}

std::string IndexBuffer::GenerateUID_Internal(const std::string& poolTag)
{
    using namespace std::string_literals;
    return typeid(IndexBuffer).name() + "#"s + poolTag;
}
