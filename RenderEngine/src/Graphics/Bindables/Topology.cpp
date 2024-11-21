#include "Topology.h"

#include "Graphics/RenderResourcePool.h"

using namespace RenderResource;

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
    : m_Type(type)
{}

void Topology::Bind() noexcept
{
    GetContext()->IASetPrimitiveTopology(m_Type);
}

std::shared_ptr<Topology> Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY type)
{
    return BindablePool::Resolve<Topology>(type);
}

std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
{
    using namespace std::string_literals;
    return typeid(Topology).name() + "#"s + std::to_string(type);
}
