#include "Topology.h"

#include "Graphics/RenderResourcePool.h"

using namespace RenderResource;

Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
    : m_Type(type)
{}

void Topology::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetPrimitiveTopology(m_Type);
}

std::shared_ptr<Topology> Topology::Resolve(Graphics& gfx,
    D3D11_PRIMITIVE_TOPOLOGY type)
{
    return Pool::Resolve<Topology>(gfx, type);
}

std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
{
    using namespace std::string_literals;
    return typeid(Topology).name() + "#"s + std::to_string(type);
}
