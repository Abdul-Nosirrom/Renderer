﻿#pragma once
#include "IBindable.h"
#include "Primitives/VertexFactory.h"
#include <memory>

//struct VertexFactory;

namespace RenderResource
{
    class IndexBuffer : public IBindable
    {
    public:
        IndexBuffer(const std::string& poolTag, const VertexFactory& indices);

        UINT GetCount() const noexcept { return m_Count; }
        void Bind() noexcept override;

        static std::shared_ptr<IndexBuffer> Resolve(const std::string& poolTag, const VertexFactory& indices);

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
        UINT m_Count;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
    };
}
