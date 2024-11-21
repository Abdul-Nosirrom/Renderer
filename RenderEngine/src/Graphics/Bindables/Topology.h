#pragma once
#include <memory>

#include "IBindable.h"

namespace RenderResource
{
    class Topology : public IBindable
    {
    public:
        Topology(D3D11_PRIMITIVE_TOPOLOGY type);
        void Bind() noexcept override;

        static std::shared_ptr<Topology> Resolve(D3D11_PRIMITIVE_TOPOLOGY type);
        static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);

        std::string GetUID() const noexcept override { return GenerateUID(m_Type); }

    protected:
        D3D11_PRIMITIVE_TOPOLOGY m_Type;
    };
}
