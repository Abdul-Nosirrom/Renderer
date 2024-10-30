#pragma once
#include "IBindable.h"
#include <memory>

struct VertexFactory;

// TODO: Figure out VertexType
namespace RenderResource
{
    class VertexBuffer : public IBindable
    {
    public:
        VertexBuffer(Graphics& gfx, const std::string& poolTag, const VertexFactory& vertexData);

        void Bind(Graphics& gfx) noexcept override;

        static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& poolTag, const VertexFactory& vertexData);
        // NOTE: this is needed so the signature matches with Resolve but we don't wanna pass the rest into it
        template<typename...Ignore>
        static std::string GenerateUID(const std::string& poolTag, Ignore&&...ignore)
        {
            return GenerateUID_Internal(poolTag);
        }
        
        std::string GetUID() const noexcept override { return GenerateUID_Internal(m_PoolTag); }

    private:
        static std::string GenerateUID_Internal(const std::string& poolTag);

    protected:
        std::string m_PoolTag;
        UINT m_Stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    };
}
